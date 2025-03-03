#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <time.h>  // for time tracking

// declares functions
int check_args(int argc, char **argv);
void initialise_vector(int vector[], int size);
double to_second_float(struct timespec in_time);
struct timespec calculate_runtime(struct timespec start_time, struct timespec end_time);

// Main program
int main(int argc, char **argv)
{
    // initialises MPI
    MPI_Init(&argc, &argv);

    // declares variables
    int rank, num_proc, num_arg, chunk, remainder;
    int *my_vector, *local_vector, my_sum = 0, final_sum = 0;
    struct timespec start_time, end_time, time_diff;
    double runtime = 0.0;

    // gets rank of current process and total number of processes
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num_proc);

    // root checks arguments and initialises vector
    if (rank == 0)
    {
        num_arg = check_args(argc, argv);
    }

    // broadcasts vector size to all processes
    MPI_Bcast(&num_arg, 1, MPI_INT, 0, MPI_COMM_WORLD);

    // allocates memory for the vector on all processes
    my_vector = malloc(num_arg * sizeof(int));

    // root process initialises the vector
    if (rank == 0)
    {
        initialise_vector(my_vector, num_arg);
    }

    // Calculate the chunk size for each process (including handling remainders)
    chunk = num_arg / num_proc;
    remainder = num_arg % num_proc;

    // Allocate memory for the local vector for each process
    local_vector = malloc((chunk + (rank < remainder ? 1 : 0)) * sizeof(int));

    // Start the time tracking for MPI_Scatter
    timespec_get(&start_time, TIME_UTC); // Capture start time for scatter operation

    // Scatter the vector to all processes
    MPI_Scatter(my_vector, chunk + (rank < remainder ? 1 : 0), MPI_INT, local_vector, chunk + (rank < remainder ? 1 : 0), MPI_INT, 0, MPI_COMM_WORLD);

    // End the time tracking for MPI_Scatter
    timespec_get(&end_time, TIME_UTC); // Capture end time for scatter operation

    // Calculate runtime for MPI_Scatter
    time_diff = calculate_runtime(start_time, end_time);
    runtime = to_second_float(time_diff);

    // calculates the local sum for this chunk
    for (int i = 0; i < chunk + (rank < remainder ? 1 : 0); i++)
    {
        my_sum += local_vector[i];
    }

    // root process calculates the final sum by receiving results from others
    if (rank == 0)
    {
        final_sum = my_sum;  // initialise final sum with the sum from the root
        for (int i = 1; i < num_proc; i++)
        {
            // Receive the local sums from other processes using MPI_Recv
            int received_sum;
            MPI_Recv(&received_sum, 1, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            final_sum += received_sum;
        }

        // print the final sum
        printf("Final sum: %d\n", final_sum);
    }
    else
    {
        // Send local sum to root process
        MPI_Send(&my_sum, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }

    // Print runtime for the scatter operation
    if (rank == 0)
    {
        printf("Runtime for MPI_Scatter: %lf seconds.\n", runtime);
    }

    // free the memory used for the vector
    free(my_vector);
    free(local_vector);

    // finalise MPI
    MPI_Finalize();

    return 0;
}

// Convert timespec to seconds as a float
double to_second_float(struct timespec in_time)
{
    double out_time = 0.0;
    long int seconds = in_time.tv_sec;
    long int nanoseconds = in_time.tv_nsec;

    out_time = seconds + nanoseconds / 1e9; // Convert to seconds
    return out_time;
}

// Calculate the difference between start and end times
struct timespec calculate_runtime(struct timespec start_time, struct timespec end_time)
{
    struct timespec time_diff;
    long int seconds = end_time.tv_sec - start_time.tv_sec;
    long int nanoseconds = end_time.tv_nsec - start_time.tv_nsec;

    if (nanoseconds < 0)
    {
        seconds -= 1;
        nanoseconds += 1000000000; // Carry the 1
    }

    time_diff.tv_sec = seconds;
    time_diff.tv_nsec = nanoseconds;

    return time_diff;
}

// Function to initialise the vector with square of the index values
void initialise_vector(int vector[], int size)
{
    for (int i = 0; i < size; i++)
    {
        vector[i] = i * i;  // stores square of index
    }
}

// Function to check arguments
int check_args(int argc, char **argv)
{
    int num_arg = 0;

    if (argc == 2)
    {
        num_arg = atoi(argv[1]);
    }
    else
    {
        fprintf(stderr, "ERROR: You did not provide a numerical argument!\n");
        fprintf(stderr, "Correct use: %s [NUMBER]\n", argv[0]);
        exit(-1);
    }

    return num_arg;
}

