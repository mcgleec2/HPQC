Practical 3 Results


Part 1 Results:

Part 1 sees this use of two programs: hello_mpi1.c and hello_mpi_serial1.c. The former is a parallel program to print “Hello, World! From process x of 16 processes”, where x is dependent on the number of processes chosen as the input. The latter program is serial code for the same core function as that of hello_mpi1.c. The serial code prints, “Hello, I am x of y” based on the number of the process used for the total number of processes, of which is the input for the program. 

hello_mpi1.c results:
This program contains a flaw with input of 0 which results in the same output if the input was 16. It prints the 16 lines, one for each process, and results in similar runtimes as when 16 is the input. This program was tested for inputs of: 0, 1, 4, 8, 12, and 16. The real time quickly jumps up to ~0.0450s but remains at this level regardless of higher inputs. The user and sys times, however, for an input of 1; are less than 0.0100s and raise roughly 0.0100s from an input of 4 to 8 to 12 etc. At an input of 16 the user and sys times of 0m0.444s and 0m0.440s, combined are more than double the real time of 0m0.469s. As the relative complexity of the program increases, the effects of the parallel code are seen as the real time is diminished by the multiple processes acting at once.

hello_mpi_serial.c results:
This program is a serial one and for an input of 1, will print “Hello, I am 0 of 1” with a real, user, and sys time of 0m0.003s, 0m0.002s, 0m0.000s respectively. These times remain similar, with an input of 16 giving a real, user, and sys time of 0m0.002s, 0m0.002s, 0m0.001s. The simple program does not seem to take much of a toll on the timing, even at the added work given by greater inputs.


Part 2 Results:

Breakdown of proof.c program

Overall & Summary
Initialises MPI and divides the work between a root process (rank 0) and client processes. Clients send a value, (rank*num_arg), to the root. Root collects the values, sums them up, and prints the result. Program also ensures the correct number of processes and arguments are given.

Header files & Function Declarations
Stdio.h for input/output functions. Stdlib.h for utility functions: atoi and exit. Mpi.h for MPI functions. Root_task: root process (rank 0); collects messages passed from processes. Client_task: non-root processes that send the messages to the root. Check_args: checks if the correct arguments are given. Check_uni_size: Make sure the right number of processors for the purpose. Check_task: Distinguishes between assigning process as root or client.

 main Function
check_args(argc, argv): Checks if the argument is a number. MPI_Init(&argc. &argv): Intialises MPI environment. MPI_Comm_rank(MPI_COMM_WORLD, &my_rank): Gets the rank of the current process in the communicator. MPI_Comm_size(MPI_COMM_WORLD, &uni_size): Gets the total number of processes in the communicator. check_uni_size(uni_size): Make sure there's a valid number of processes. check_task(uni_size, my_rank, num_arg): Decides which task (root or client) to run based on the rank. MPI_Finalize():  Finalises the MPI environment.

 root_task Function
Variables: recv_message: Stores the message received from a client. output_sum: The sum of all received values.
The root receives messages from every other process, an integer sent by each process. MPI_Recv receives the messages from the clients and the value is added to the out_sum. Root outputs the total sum of all received values.

 client_task Function
Variables:
send_message: Message to be sent to the root.
dest: Destination of the message.
The client calculates the message to send as my_rank * num_arg, my_rank is the rank of client, and MPI_Send sends the message to the root process.

 check_args function:
Function which checks if exactly one argument is provided and converts it from a string to an integer (atoi). If the argument isn’t incorrect, it outputs an error message and exits the program.

 check_uni_size function:
Function that checks if there are enough processes to run the program. If the number of processes (uni_size) is less than 1, it outputs an error and exits.

 check_task function:
Function that decides what task each process does based on its rank. If the rank is 0 (root), it runs the root_task function, otherwise, it runs the client_task function.


Code Optimisation

The program gets messages from the clients (ranks 1 to uni_size - 1) to the root (rank = 0), which are sent by each client based on rank*num_arg and then summed up by the root. So for n processes there are processes with ranks 1 to n - 1, which is the same as 1 to uni_size -1. 1*num_arg + 2*num_arg+...+ (uni_size - 1)*num_arg. Take the num_arg outside and left with a series of 1 to n-1. The formula for the sum of n positive integers is Sn = [n(n+1)]/2. So for the program: sum of ranks = [uni_size(uni_size+1)]/2. And for the total sum: Total sum = num_arg * {[uni_size * (uni_size + 1)]/2}. So using this mathematical operation should get the same result as that of the original program.



Part 3 Results:

Break down of vector_serial.c

Headers
Use input/output library and standard library to allow certain functions etc to be used/done.

Functions Declarations
Declare functions to be used in code.

Main Function
Use arguments that are checked to only have one argument. Allocates memory for an array of integers, the number of elements of which is specified. Initialises the elements of the vector to 0 and loops through, setting each element to 0. Then sums up the elements in the vector and returns the sum. Finally, prints the sum to the screen and frees the memory. 

Function Definitions
Defines sum_vector as a function which has a vector and a size. Initializes the sum to 0 then iterates through the vector, adding the elements’ values to the sum. Returns this sum. Defines initialise_vector as a function to initialise all elements of the vector to 0, then loop through the vector and assign each element to the initial value. Defines print_vector as a function to print each element of the vector, loops through as well and prints the values. Defines check_args as a function to ensure only 2 arguments (program name and numerical argument) are present. Converts argv[i] into an integer using atoi then stores it in num_arg; also has an error message to alert of an incorrect number of arguments and exit the program.



Benchmarking Results:

Vector_serial.c:
Benchmarking of the vector_serial.c program results in similar times for all values of real, user, and sys across a range of inputs (0,1,5,9,10,50,100,1000). For an input of 1: real = 0.002s, user = 0.002s , and sys = 0.000s; while for 10: real = 0.004s, user = 0.002s , and sys = 0.000s;  and for 1000: real = 0.004s, user = 0.000s , and sys = 0.002s. This stands to reason as the program sets all vector elements to 0 so the sum will always be 0 and thus putting minimal demand on any form of time parameter. 

Vector_serial1.c:
This program was benchmarked both internally and externally from inputs ranging from 1-1000. The runtime calculated internally found times of 1ms for inputs of 3,5, and 10. These increased to 13ms for input of 1000. The real, user, and sys times for these varied slightly with mostly similar times of 0.002s for real and user and mostly 0.000s for sys. An input of 1861 is the highest the program allows before overflow errors occur. 1861 outputs “Final sum: 2146682110” with runtime 23 ms while an input of 1862 outputs “Final sum:  -2144821865” with runtime of 23 ms again. 

Vector_serial2.c: 
This program was created in an attempt to create a more complex version of its predecessor by multiplying the vector elements with one another instead of summing them. While the serial version appears to work as intended, a parallel version proved too difficult to implement at this time.

Vector_parallel1.c: 
Benchmarking of the parallel.c program results in a progressive increase in user and sys times, whereas the real time remains relatively similar even with greater number of elements inputted for the vector. A wide range of values were used for the number of processes and the number of elements in the vectors, these numbers were changed in many combinations, i.e. -np 1 for 1 to -np 1 for 20 and -np 4 for 4 to -np 16 for 4, etc. A clear need for appropriate programs being used for parallel is seen across this testing, e.g. 1 processor with 1 element gives  real = 0.374s, user = 0..069s , and sys = 0.072s versus 16 processors with 1 element giving  real = 0.476s, user = 0..489s , and sys = 0.391s for the same task and result of 0.

Serial vs parallel:
Based on these times of real for both parallel and serial, it does not appear that there is a valid input, free of overflow errors, where the parallel’s real time is less than that of the serial’s real time. 

