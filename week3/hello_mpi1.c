// include the standard input/output library
#include <stdio.h>

// include the MPI header file
#include <mpi.h>

// define the main() function to return an int
int main(int argc, char *argv[])
{
	// initialise the MPI environment
	MPI_Init(&argc,& argv);

	// get the rank of the process
	int rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	// get the total number of processes
	int size;
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	// print "Hello, World!" with the rank of each process
	printf("Hello, World! from process %d out of %d processes\n", rank, size);

	// finalise the MPI environment
	MPI_Finalize();

	return 0; // return 0 by default
}
