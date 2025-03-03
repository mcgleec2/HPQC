#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <time.h>  // for time tracking

// Function to initialise the vector with square of the index values
void initialise_vector(int vector[], int size)
{
    for (int i = 0; i < size; i++)
    {
        vector[i] = i * i;  // stores square of index
    }
}

// Custom sum function
void custom_sum(void *invec, void *inoutvec, int *len, MPI_Datatype *dtype)
{
    int *in_array = (int *)invec;
    int *inout_array = (int *)inoutvec;
    for (int i = 0; i < *len; i++)
    {
        inout_array[i] += in_array[i];
    }
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

int main(int argc, char **argv)
{
    // initialises MPI
    MPI_Init(&argc, &argv);

    // declares variables
    int rank, num_proc, num_arg, chunk, *my_vector, *local_vector;
    int *final_vector, final_sum = 0;
    struct timespec start_time, end_time, time_diff;
    double runtime_custom = 0.0, runtime_predefined = 0.0;

    // gets rank of current process and total number of processes
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num_proc);

    // root checks arguments and initialises vector
    if (rank == 0)
    {
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

    // Allocate memory for the local vector
    chunk = num_arg / num_proc;
    local_vector = malloc(chunk * sizeof(int));

    // Scatter the vector to all processes
    MPI_Scatter(my_vector, chunk, MPI_INT, local_vector, chunk, MPI_INT, 0, MPI_COMM_WORLD);

    // Predefined MPI_SUM
    timespec_get(&start_time, TIME_UTC); // Capture start time for predefined MPI_SUM
    MPI_Reduce(local_vector, &final_sum, chunk, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    timespec_get(&end_time, TIME_UTC); // Capture end time for predefined MPI_SUM

    // Calculate runtime for predefined MPI_SUM
    time_diff = calculate_runtime(start_time, end_time);
    runtime_predefined = to_second_float(time_diff);

    // Print final sum using predefined MPI_SUM on the root process
    if (rank == 0)
    {
        printf("Final sum using MPI_SUM: %d\n", final_sum);
    }

    // Create custom MPI operation for summing arrays
    MPI_Op custom_op;
    MPI_Op_create(&custom_sum, 1, &custom_op);

    // Custom sum using the custom operation
    final_sum = 0; // reset final sum
    timespec_get(&start_time, TIME_UTC); // Capture start time for custom sum
    MPI_Reduce(local_vector, &final_sum, chunk, MPI_INT, custom_op, 0, MPI_COMM_WORLD);
    timespec_get(&end_time, TIME_UTC); // Capture end time for custom sum

    // Calculate runtime for custom sum
    time_diff = calculate_runtime(start_time, end_time);
    runtime_custom = to_second_float(time_diff);

    // Print final sum using custom sum on the root process
    if (rank == 0)
    {
        printf("Final sum using custom sum operation: %d\n", final_sum);
    }

    // Print comparison of runtimes
    if (rank == 0)
    {
        printf("Runtime for predefined MPI_SUM: %lf seconds.\n", runtime_predefined);
        printf("Runtime for custom sum operation: %lf seconds.\n", runtime_custom);
    }

    // Free up
    free(my_vector);
    free(local_vector);

    // Free the custom operation
    MPI_Op_free(&custom_op);

    // finalise MPI
    MPI_Finalize();

    return 0;
}

