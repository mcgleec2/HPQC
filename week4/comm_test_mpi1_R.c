// Using Rsend()

#include <stdio.h>
#include <mpi.h>
#include <time.h>

// functions
double to_second_float(struct timespec in_time);
struct timespec calculate_runtime(struct timespec start_time, struct timespec end_time);
void initialize_mpi(int *argc, char ***argv, int *my_rank, int *uni_size);
void finalize_mpi();
void send_message_to_root(int my_rank, int uni_size);
void receive_message_from_others(int my_rank, int uni_size);
void handle_communication(int my_rank, int uni_size);

int main(int argc, char **argv)
{
    int my_rank, uni_size;

    // initialise mpi
    initialize_mpi(&argc, &argv, &my_rank, &uni_size);

    // handle communication between processes
    handle_communication(my_rank, uni_size);

    // finalise mpi
    finalize_mpi();

    return 0;
}

void initialize_mpi(int *argc, char ***argv, int *my_rank, int *uni_size)
{
    // initialise mpi environment
    MPI_Init(argc, argv);

    // get rank of the current process
    MPI_Comm_rank(MPI_COMM_WORLD, my_rank);

    // get total number of processes
    MPI_Comm_size(MPI_COMM_WORLD, uni_size);
}

void finalize_mpi()
{
    // finalise mpi environment
    MPI_Finalize();
}

void send_message_to_root(int my_rank, int uni_size)
{
    struct timespec start_time, end_time, time_diff;
    double runtime = 0.0;

    int send_message = my_rank * 10;
    int dest = 0;
    int tag = 0;
    int count = 1;

    // start time tracking
    timespec_get(&start_time, TIME_UTC);

    // send message to root process using mpi_rsend
    MPI_Rsend(&send_message, count, MPI_INT, dest, tag, MPI_COMM_WORLD);

    // end time tracking
    timespec_get(&end_time, TIME_UTC);

    // calculate time difference
    time_diff = calculate_runtime(start_time, end_time);
    runtime = to_second_float(time_diff);

    // print the time taken for sending the message
    printf("[rank %d] took %lf seconds to send message to rank %d\n", my_rank, runtime, dest);
}

void receive_message_from_others(int my_rank, int uni_size)
{
    struct timespec start_time, end_time, time_diff;
    double runtime = 0.0;

    int recv_message;
    int source;
    int count = 1;
    int tag = 0;
    MPI_Status status;

    // start time tracking
    timespec_get(&start_time, TIME_UTC);

    // iterate through all ranks except root to receive messages
    for (int their_rank = 1; their_rank < uni_size; their_rank++)
    {
        source = their_rank;
        MPI_Recv(&recv_message, count, MPI_INT, source, tag, MPI_COMM_WORLD, &status);
        printf("hello, i am %d of %d. received %d from rank %d\n", my_rank, uni_size, recv_message, source);
    }

    // end time tracking
    timespec_get(&end_time, TIME_UTC);

    // calculate time difference
    time_diff = calculate_runtime(start_time, end_time);
    runtime = to_second_float(time_diff);

    // print the time taken for receiving messages
    printf("[rank %d] took %lf seconds to receive messages\n", my_rank, runtime);
}

void handle_communication(int my_rank, int uni_size)
{
    if (uni_size > 1)
    {
        if (my_rank == 0)
        {
            // root process receives messages
            receive_message_from_others(my_rank, uni_size);
        }
        else
        {
            // non-root processes send messages
            send_message_to_root(my_rank, uni_size);
        }
    }
    else
    {
        // print warning if there are not enough processes
        printf("unable to communicate with less than 2 processes. mpi communicator size = %d\n", uni_size);
    }
}

// convert timespec to seconds as a float
double to_second_float(struct timespec in_time)
{
    double out_time = in_time.tv_sec + in_time.tv_nsec / 1e9;
    return out_time;
}

// calculate the difference between start and end times
struct timespec calculate_runtime(struct timespec start_time, struct timespec end_time)
{
    struct timespec time_diff;
    time_diff.tv_sec = end_time.tv_sec - start_time.tv_sec;
    time_diff.tv_nsec = end_time.tv_nsec - start_time.tv_nsec;

    // handle negative nanoseconds by borrowing from seconds
    if (time_diff.tv_nsec < 0)
    {
        time_diff.tv_sec -= 1;
        time_diff.tv_nsec += 1000000000;
    }

    return time_diff;
}
