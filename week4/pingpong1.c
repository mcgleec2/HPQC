#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <time.h>

// Function to convert timespec to seconds as a float
double to_second_float(struct timespec in_time) {
    double out_time = 0.0;
    long int seconds = in_time.tv_sec;
    long int nanoseconds = in_time.tv_nsec;
    out_time = seconds + nanoseconds / 1e9; // Convert to seconds
    return out_time;
}

// Function to calculate the runtime difference between start and end times
struct timespec calculate_runtime(struct timespec start_time, struct timespec end_time) {
    struct timespec time_diff;
    long int seconds = end_time.tv_sec - start_time.tv_sec;
    long int nanoseconds = end_time.tv_nsec - start_time.tv_nsec;

    if (nanoseconds < 0) {
        seconds -= 1;
        nanoseconds += 1000000000; // Carry the 1
    }

    time_diff.tv_sec = seconds;
    time_diff.tv_nsec = nanoseconds;

    return time_diff;
}

int main(int argc, char *argv[]) {
    int rank, size;
    int num_pings;
    int counter = 0;
    struct timespec start_time, end_time, time_diff;
    double elapsed_time, average_time;

    // initialise MPI
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (argc != 3) {
        if (rank == 0) {
            printf("Usage: %s <num_pings> <array_size_in_bytes>\n", argv[0]);
        }
        MPI_Finalize();
        return 1;
    }

    // gets number of pings and array size from arguments (only on root process)
    if (rank == 0) {
        num_pings = atoi(argv[1]);
    }

    // broadcasts number of pings to all processes
    MPI_Bcast(&num_pings, 1, MPI_INT, 0, MPI_COMM_WORLD);

    // allocates the array dynamically based on the size argument
    size_t array_size = atoi(argv[2]);
    int num_elements = array_size / sizeof(int);  // calculates number of elements
    int *array = (int *)malloc(array_size);

    if (array == NULL) {
        if (rank == 0) {
            printf("Memory allocation failed for array\n");
        }
        MPI_Finalize();
        return 1;
    }

    // initialises array with some values (1st element is counter)
    for (int i = 0; i < num_elements; i++) {
        array[i] = 0;
    }

    // times the process
    timespec_get(&start_time, TIME_UTC); // Capture start time

    // pingpong loop
    while (counter < num_pings) {
        if (rank == 0) {  // root
            // sends array to the client
            MPI_Send(array, num_elements, MPI_INT, 1, 0, MPI_COMM_WORLD);
            // receives array from client
            MPI_Recv(array, num_elements, MPI_INT, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        } else if (rank == 1) {  // client
            // receives array from root
            MPI_Recv(array, num_elements, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            // increments the counter (stored in array[0])
            array[0]++;
            // sends array back to root
            MPI_Send(array, num_elements, MPI_INT, 0, 0, MPI_COMM_WORLD);
        }

        // updates counter for each successful send/receive
        counter++;
    }

    // End the time tracking
    timespec_get(&end_time, TIME_UTC); // Capture end time

    // Calculate elapsed time
    time_diff = calculate_runtime(start_time, end_time);
    elapsed_time = to_second_float(time_diff);

    // Calculate average time per ping-pong
    average_time = elapsed_time / num_pings;

    // Root prints the results
    if (rank == 0) {
        printf("Final counter value: %d\n", array[0]);
        printf("Elapsed time: %lf seconds\n", elapsed_time);
        printf("Average time per ping-pong: %lf seconds\n", average_time);
    }

    // Free allocated memory
    free(array);

    // Finalize MPI
    MPI_Finalize();
    return 0;
}
