// Using MPI_Isend()

#include <stdio.h>
#include <mpi.h>
#include <time.h>

// functions
void initialize_mpi(int *argc, char ***argv, int *my_rank, int *uni_size);
void finalize_mpi();
void send_message_to_root(int my_rank, int uni_size);
void receive_message_from_others(int my_rank, int uni_size);
void handle_communication(int my_rank, int uni_size);
double to_second_float(struct timespec in_time);
struct timespec calculate_runtime(struct timespec start_time, struct timespec end_time);

int main(int argc, char **argv)
{
    // declares variables
    int my_rank, uni_size;

    // initialises MPI
    initialize_mpi(&argc, &argv, &my_rank, &uni_size);

    // handles communication (send/receive messages)
    handle_communication(my_rank, uni_size);

    // finalises MPI
    finalize_mpi();

    return 0;
}

// Function to initialise MPI
void initialize_mpi(int *argc, char ***argv, int *my_rank, int *uni_size)
{
    int ierror = MPI_Init(argc, argv);

    // gets rank and world size
    ierror = MPI_Comm_rank(MPI_COMM_WORLD, my_rank);
    ierror = MPI_Comm_size(MPI_COMM_WORLD, uni_size);
}

// Function to finalise MPI
void finalize_mpi()
{
    int ierror = MPI_Finalize();
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

// Function to handle sending messages from non-root processes using MPI_Isend()
void send_message_to_root(int my_rank, int uni_size)
{
    int send_message = my_rank * 10; // creates the message to send
    int dest = 0;                    // root rank is 0
    int tag = 0;
    int count = 1;

    MPI_Request request;  // declares an MPI request for non-blocking send

    struct timespec start_time, end_time, time_diff;
    timespec_get(&start_time, TIME_UTC); // capture start time

    // Sends message to rank 0 using MPI_Isend (non-blocking send)
    MPI_Isend(&send_message, count, MPI_INT, dest, tag, MPI_COMM_WORLD, &request);

    // Waits for send to complete
    MPI_Wait(&request, MPI_STATUS_IGNORE);

    timespec_get(&end_time, TIME_UTC); // Capture end time

    // Calculate runtime
    time_diff = calculate_runtime(start_time, end_time);
    double runtime = to_second_float(time_diff);

    // Prints the runtime for sending the message
    printf("Rank %d took %lf seconds to send message to Rank %d.\n", my_rank, runtime, dest);
}

// Function to handle receiving messages from other processes
void receive_message_from_others(int my_rank, int uni_size)
{
    int recv_message;
    int source;
    int count = 1;
    int tag = 0;
    MPI_Status status;

    struct timespec start_time, end_time, time_diff;

    // Iterates through all ranks except root (rank 0)
    for (int their_rank = 1; their_rank < uni_size; their_rank++)
    {
        source = their_rank; // Sets source rank of the message

        timespec_get(&start_time, TIME_UTC); // Capture start time

        // Receives the message
        MPI_Recv(&recv_message, count, MPI_INT, source, tag, MPI_COMM_WORLD, &status);

        timespec_get(&end_time, TIME_UTC); // Capture end time

        // Calculate runtime
        time_diff = calculate_runtime(start_time, end_time);
        double runtime = to_second_float(time_diff);

        // Prints the received message and the time taken
        printf("Rank %d took %lf seconds to receive message from Rank %d. Received %d\n", my_rank, runtime, source, recv_message);
    }
}

// Function to handle communication
void handle_communication(int my_rank, int uni_size)
{
    if (uni_size > 1)
    {
        if (my_rank == 0)
        {
            // If rank is 0 (root), receives messages from other ranks
            receive_message_from_others(my_rank, uni_size);
        }
        else
        {
            // If rank is not 0 (not root), sends message to rank 0
            send_message_to_root(my_rank, uni_size);
        }
    }
    else
    {
        // Prints a warning
        printf("Unable to communicate with less than 2 processes. MPI communicator size = %d\n", uni_size);
    }
}

