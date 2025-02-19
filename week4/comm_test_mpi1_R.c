// Using MPI_Rsend()

#include <stdio.h>
#include <mpi.h>

// functions
void initialize_mpi(int *argc, char ***argv, int *my_rank, int *uni_size);
void finalize_mpi();
void send_message_to_root(int my_rank, int uni_size);
void receive_message_from_others(int my_rank, int uni_size);
void handle_communication(int my_rank, int uni_size);

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

// function to initialise MPI
void initialize_mpi(int *argc, char ***argv, int *my_rank, int *uni_size)
{
    int ierror = MPI_Init(argc, argv);

    // gets rank and world size
    ierror = MPI_Comm_rank(MPI_COMM_WORLD, my_rank);
    ierror = MPI_Comm_size(MPI_COMM_WORLD, uni_size);
}

// function to finalise MPI
void finalize_mpi()
{
    int ierror = MPI_Finalize();
}

// function to handle sending messages from non-root processes using MPI_Rsend()
void send_message_to_root(int my_rank, int uni_size)
{
    int send_message = my_rank * 10; // creates the message to send
    int dest = 0;                    // root rank is 0
    int tag = 0;
    int count = 1;

    // root process calls MPI_Recv() beforehand: needs to be ready to receive before sending

    // sends message to rank 0 using MPI_Rsend (ready send)
    MPI_Rsend(&send_message, count, MPI_INT, dest, tag, MPI_COMM_WORLD);

    // prints message
    printf("Hello, I am %d of %d. Sent %d to Rank %d\n", my_rank, uni_size, send_message, dest);
}

// function to handle receiving messages from other processes
void receive_message_from_others(int my_rank, int uni_size)
{
    int recv_message;
    int source;
    int count = 1;
    int tag = 0;
    MPI_Status status;

    // iterates through all ranks except root (rank 0)
    for (int their_rank = 1; their_rank < uni_size; their_rank++)
    {
        source = their_rank; // sets source rank of the message

        // receives the message
        MPI_Recv(&recv_message, count, MPI_INT, source, tag, MPI_COMM_WORLD, &status);

        // prints the received message
        printf("Hello, I am %d of %d. Received %d from Rank %d\n", my_rank, uni_size, recv_message, source);
    }
}

// function to handle communication
void handle_communication(int my_rank, int uni_size)
{
    if (uni_size > 1)
    {
        if (my_rank == 0)
        {
            // if rank is 0 (root), receives messages from other ranks
            receive_message_from_others(my_rank, uni_size);
        }
        else
        {
            // if rank is not 0 (not root), sends message to rank 0
            send_message_to_root(my_rank, uni_size);
        }
    }
    else
    {
        // prints a warning
        printf("Unable to communicate with less than 2 processes. MPI communicator size = %d\n", uni_size);
    }
}

