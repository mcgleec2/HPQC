// Modified program of vector_serial1.c to parallise the serial program

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

// declares functions
int check_args(int argc, char **argv);
void initialise_vector(int vector[], int size);
int sum_vector(int vector[], int size);

int main(int argc, char **argv)
{
    // initialises MPI
    MPI_Init(&argc, &argv);

    // declares variables
    int rank, num_proc, num_arg, chunk, start, stop;
    int *my_vector, *sums_array, my_sum = 0, final_sum = 0;

    // gets rank of current process and  total number of processes
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num_proc);

    // root checks arguments and intialises vector
    if (rank == 0)
    {
        num_arg = check_args(argc, argv);
    }

    // broadcasts vector size to all processes
    MPI_Bcast(&num_arg, 1, MPI_INT, 0, MPI_COMM_WORLD);

    // allocates memory for the vector on all processes
    my_vector = malloc(num_arg * sizeof(int));

    // initialises the vector on the root process and splits it to all processes
    if (rank == 0)
    {
        initialise_vector(my_vector, num_arg);
    }

    // splits the vector data across all processes
    MPI_Bcast(my_vector, num_arg, MPI_INT, 0, MPI_COMM_WORLD);

    // calculates chunk size for each process
    chunk = num_arg / num_proc;
    start = rank * chunk;
    stop = (rank + 1) * chunk;

    // calculates the local sum for this chunk
    for (int i = start; i < stop; i++)
    {
        my_sum += my_vector[i];
    }

    // gets  all local  sums at the root process
    if (rank == 0)
    {
        sums_array = malloc(num_proc * sizeof(int));
    }

    // gets the local sums from all processes to the root
    MPI_Gather(&my_sum, 1, MPI_INT, sums_array, 1, MPI_INT, 0, MPI_COMM_WORLD);

    // root process calculates the final sum
    if (rank == 0)
    {
	final_sum = 0;	// initialises final sum
        for (int i = 0; i < num_proc; i++)
	    {
            final_sum += sums_array[i];
            }

        // print the final sum
        printf("Final sum: %d\n", final_sum);

        // free the memory used for the sum array
        free(sums_array);
    }

    // free the memory used for the vector
    free(my_vector);

    // finalize MPI
    MPI_Finalize();

    return 0;
}

// function to sum the vector
int sum_vector(int vector[], int size)
{
    int sum = 0;
    for (int i = 0; i < size; i++) {
        sum += vector[i];
    }
    return sum;
}

// function to initialize the vector with square of the index values
void initialise_vector(int vector[], int size)
{
    for (int i = 0; i < size; i++) {
        vector[i] = i * i;  // stores square of index
    }
}

// function to print the vector
void print_vector(int vector[], int size)
{
    for (int i = 0; i < size; i++) {
        printf("%d\n", vector[i]);
    }
}

// function to check arguments
int check_args(int argc, char **argv)
{
    int num_arg = 0;

    if (argc == 2) {
        num_arg = atoi(argv[1]);
    } else {
        fprintf(stderr, "ERROR: You did not provide a numerical argument!\n");
        fprintf(stderr, "Correct use: %s [NUMBER]\n", argv[0]);
        exit(-1);
    }

    return num_arg;
}

