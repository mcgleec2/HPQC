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

    if (argc != 2) {
        if (rank == 0) {
            printf("Usage: %s <num_pings>\n", argv[0]);
        }
        MPI_Finalize();
        return 1;
    }

    // gets number of pings from arguments (only on root process)
    if (rank == 0) {
        num_pings = atoi(argv[1]);
    }

    // broadcasts number of pings to all processes
    MPI_Bcast(&num_pings, 1, MPI_INT, 0, MPI_COMM_WORLD);

    // Timing the process
    timespec_get(&start_time, TIME_UTC); // Capture start time

    // pingpong loop
    while (counter < num_pings) {
        if (rank == 0) {  // root
            // sends ping to the client
            MPI_Send(&counter, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
            // receives pong from client
            MPI_Recv(&counter, 1, MPI_INT, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        } else if (rank == 1) {  // client
            // receives the ping from root
            MPI_Recv(&counter, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            // increments the counter
            counter++;
            // sends pong back to root
            MPI_Send(&counter, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        }
    }

    // End the time tracking
    timespec_get(&end_time, TIME_UTC); // Capture end time

    // Calculates elapsed time
    time_diff = calculate_runtime(start_time, end_time);
    elapsed_time = to_second_float(time_diff);

    // calculates average time
    average_time = elapsed_time / num_pings;

    // root prints results
    if (rank == 0) {
        printf("Final counter value: %d\n", counter);
        printf("Elapsed time: %lf seconds\n", elapsed_time);
        printf("Average time per ping-pong: %lf seconds\n", average_time);
    }

    // finalise MPI
    MPI_Finalize();
    return 0;
}


