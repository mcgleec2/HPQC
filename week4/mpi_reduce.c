#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <time.h>  // for time tracking

// declares functions
int check_args(int argc, char **argv);
void initialise_vector(int vector[], int size);
double to_second_float(struct timespec in_time);
struct timespec calculate_runtime(struct timespec start_time, struct timespec end_time);

// Main
int main(int argc, char **argv)
{
    // initialises MPI
    MPI_Init(&argc, &argv);

    // declares variables
    int rank, num_proc, num_arg, chunk, start, stop;
    int *my_vector, my_sum = 0, final_sum = 0, remainder;
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

    // broadcasts vector size to all processes (using MPI_Send and MPI_Recv)
    if (rank == 0)
    {
        for (int i = 1; i < num_proc; i++)
        {
            MPI_Send(&num_arg, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
        }
    }
    else
    {
        MPI_Recv(&num_arg, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }

    // allocates memory for the vector on all processes
    my_vector = malloc(num_arg * sizeof(int));

    // initialises the vector on the root process
    if (rank == 0)
    {
        initialise_vector(my_vector, num_arg);
    }

    // Start the time tracking for MPI_Send and MPI_Recv
    timespec_get(&start_time, TIME_UTC); // Capture start time for sending vector

    // Root process scatters the vector to all processes
    MPI_Bcast(&my_vector[0], num_arg, MPI_INT, 0, MPI_COMM_WORLD);

    // End the time tracking for MPI_Send and MPI_Recv
    timespec_get(&end_time, TIME_UTC); // Capture end time for receiving vector

    // Calculate runtime for communication
    time_diff = calculate_runtime(start_time, end_time);
    runtime = to_second_float(time_diff);

    // calculates chunk size for each process (also deals with extra elements)
    chunk = num_arg / num_proc;
    remainder = num_arg % num_proc;

    // each process handles chunk size + 1 if it's one of the first remainder processes
    start = rank * chunk + (rank < remainder ? rank : remainder);
    stop = start + chunk + (rank < remainder ? 1 : 0);

    // calculates the local sum for this chunk
    for (int i = start; i < stop; i++)
    {
        my_sum += my_vector[i];
    }

    // Use MPI_Reduce to sum the local sums from all processes
    MPI_Reduce(&my_sum, &final_sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    // The root process prints the final sum
    if (rank == 0)
    {
        // print the final sum
        printf("Final sum: %d\n", final_sum);
    }

    // Print runtime for the communication
    if (rank == 0)
    {
        printf("Runtime for MPI_Reduce: %lf seconds.\n", runtime);
    }

    // free the memory used for the vector
    free(my_vector);

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

// Function to initialize the vector with square of the index values
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

