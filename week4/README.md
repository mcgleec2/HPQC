Week 4 Practical

Program Tracking:
Part 1:
Step 1:
comm_test_mpi.c = PHY1090/week4 original program
Step 2:
comm_test_mpi1.c = Functionlised program
Step 3:
comm_test_mpi1_S.c = Modified comm_test_mpi1.c to use MPI_Ssend()
comm_test_mpi1_B.c = Modified comm_test_mpi1.c to use MPI_Bsend()
comm_test_mpi1_R.c = Modified comm_test_mpi1.c to use MPI_Rsend()
comm_test_mpi1_I.c = Modified comm_test_mpi1.c to use MPI_Isend()
Step 4:
comm_test_mpi1_S.c = Added internal benchmarking
comm_test_mpi1_B.c = Added internal benchmarking
comm_test_mpi1_R.c = Added internal benchmarking
comm_test_mpi1_I.c = Added internal benchmarking
Part 2:
Step 1:
pingpong.c
Step 2:
---
Step 3:
pingpong1.c
Part 3:
Step 1:
mpi_bcast.c
mpi_scatter.c
moi_send_rec.c
Step 2:
mpi_send_rec.c (again)
mpi_gather.c
mpi_reduce.c
Step 3:
reduce_op.c





Results:

Part 1:
Step 1:
comm_test_mpi.c
The program takes an input for the number of processors and prints out, “Hello, I am %d of %d. Received %d from Rank %d” on one line and “Hello, I am %d of %d. Sent %d to Rank %d” on another line. The greater the number of processors used the greater repetitions of these lines for different values. For example, using 2 processors gives,  “Hello, I am 0 of 2. Received 10 from Rank 1” on one line and, “Hello, I am 1 of 2. Sent 10 to Rank 0” on another line.The rank 1 process has sent a message of 10 (based on the rank number x 10 → 1x10=10) to rank 0 (root); this is described by the 1 of 2 process. The rank 0 process has received a message of 10 from rank 1 (client); this is described by the 0 of 2 process. For more processors all of the messages received by the rank are 0 of 6, for example, while each client sending the message will iterate from 1 of 6 up to 5 of 6 with its message to send. Inputting less than 2 processors to use will return an error.


Step 2:
See program in repo


Step 3:
Document errors or differences

Used 4 processors for all.

MPI_Ssend():
No obvious changes. All prints fine, randomised sent going 2, 3, then received then finally 1 sent. All received bunched together in the middle, in order. 

MPI_Bsend():
Same, random sent messages and in the middle is ordered received messages. 

MPI_Rsend():
Same, random sent messages and in the middle is ordered received messages.

MPI_Isend():
Normal, all sent messages together at top, all received messages together at bottom and ordered.

Which version is better?
None stand out in particular for these programs but MPI_Isend() would be a good choice performance wise; allowing other tasks to be completed while sending. MPI_Ssend() would be another good choice as it offers simplicity and an extra safety step by waiting until the message is sent. 


Step 4:

Document Internal Benchmarking

Ssend:
For 4 processes: Rank 0 took 0.000176 seconds to receive 10 from Rank 1; Rank 0 took 0.000237 seconds to receive 20 from Rank 2; and Rank 0 took 0.000249 seconds to receive 30 from Rank 3. While. Rank 1 took 0.000064 seconds to send 10 to Rank 0 using MPI_Ssend; Rank 2 took 0.000256 seconds to send 20 to Rank 0 using MPI_Ssend; and Rank 3 took 0.000150 seconds to send 30 to Rank 0 using MPI_Ssend.

Bsend:
For 4 processes, Rank 0 took 0.000240 seconds to receive 10 from Rank 1; Rank 0 took 0.000271 seconds to receive 20 from Rank 2; and Rank 0 took 0.000279 seconds to receive 30 from Rank 3. While, Rank 1 took 0.000016 seconds to send 10 to Rank 0 using MPI_Bsend; Rank 2 took 0.000017 seconds to send 20 to Rank 0 using MPI_Bsend; Rank 3 took 0.000017 seconds to send 30 to Rank 0 using MPI_Bsend. 

Rsend:
For 4 processes, [rank 0] took 0.000141 seconds to receive messages. While, [rank 1] took 0.000032 seconds to send message to rank 0; [rank 2] took 0.000023 seconds to send message to rank 0; and [rank 3] took 0.000040 seconds to send message to rank 0.

Isend:
For 4 processes, Rank 0 took 0.000065 seconds to receive message from Rank 1. Received 10; Rank 0 took 0.000002 seconds to receive message from Rank 2. Received 20; and Rank 0 took 0.000000 seconds to receive message from Rank 3. Received 30. While, Rank 1 took 0.000015 seconds to send message to Rank 0.; Rank 2 took 0.000017 seconds to send message to Rank 0.; and Rank 3 took 0.000024 seconds to send message to Rank 0.

Overall, the timing for sending and receiving the messages is very quick and the numbers vary quite a bit from run to run for the same and different number of processes across all the types of sends.



Part 2:
Part 2 Results

Step 1:
See program in repo

Step 2:
Document Code:
Include standard input/output library; standard library; MPI library; and time library. Use the to_second_float and calculate_runtime functions from previous programs. Use the main function to declare 2 variables for rank and size of process. Store the ping pongs in num_pings. Have the counter track the number of pingpong iterations. Create variables for time tracking. Initialise MPI environment and set up communication of ranks and sizes. Set up an argument check for errors and exit if incorrect number. Read the number of pings pongs and have root broadcast to other processes. Start the runtime, then begin the loop. The loop sends a counter to the client and waits to receive the incremented counter; while the client receives the root counter and sends back the incremented counter. Loop repeats until num_pings value of messages has been sent; finally runtime is ended. Calculate elapsed time from the difference of start and end times. Calculate average time by dividing elapsed time by number of pingpongs. Print the time calculations to the screen and close the MPI environment.

Document Results:
For an argument input of 1 the results show: Final counter value: 1; Elapsed time: 0.000075 seconds; and Average time per ping-pong: 0.000075 seconds. For an argument input of 10 the results show: Final counter value: 10; Elapsed time: 0.000056 seconds; and Average time per ping-pong: 0.000006 seconds. For an argument input of 100 the results show: Final counter value: 100; Elapsed time: 0.000245 seconds; and Average time per ping-pong: 0.000002 seconds. For an argument input of 10000 the results show: Final counter value: 10000; Elapsed time: 0.008653 seconds; and Average time per ping-pong: 0.000001 seconds. The program outputs a confirmation of the argument in the final counter value. The elapsed times increase with the increasing values of the arguments. Meanwhile, the average time per message sent and received decreases with greater values for arguments. The average time per pingpong converges towards 0.000001 seconds as the argument value increases.


Step 3:
Array sizes which matched the incremental values of 8B to 2MiB were used for the array sizes. Their values were 2^3 bytes, 2^6 bytes, 2^9 bytes, 2^12 bytes, 2^15 bytes, 2^18 bytes. And 2^21 bytes. The elapsed time taken for the data to pass was plotted against the array sizes in bytes and a linear fit applied. The corresponding elapsed times for the array sizes were 0.000583, 0.000828, 0.001157, 0.005370,  0.010029,  0.035955, 0.253403, respectively. The plot shows a clear linear trend of the elapsed time increase comparable to the array size in bytes increase. Bandwidth is the rate of data transfer and can be obtained from the inverse of the slope, giving bytes per second, as opposed to the slope which would give time per byte. Higher the slope, the more time taken, which means lower bandwidth. The latency can be determined from the y intercept, as the array size decreases the time taken becomes more dominated by this latency. 

Slope (m): 1.20e-07 seconds per byte
Intercept (c) [Latency]: 2.98e-03 seconds
Inverse Slope (1/m) [Bandwidth]: 8.37e+06 bytes per second

(Image of graph)
https://github.com/mcgleec2/HPQC/blob/main/Topic4_P2_S3.png



Part 3: 

Document Results

Step 1:

Predictions:

MPI_Broadcast(): Fastest for sending entire array to all processes

MPI_Send() and MPI_Recv(): Slowest option, multiple point-to-point communications.

MPI_Scatter(): Good for sending parts of an array across processes.Middle.


Benchmarking: 

MPI_Broadcast(): 
Input: 4
Output: 14
Runtime for mpi_Bcast: 0.000001 seconds.

MPI_Send() and MPI_Recv(): 
Input: 4
Output: 14
Runtime for mpi_Send and mpi_Recv: 0.000005 seconds.

MPI_Scatter(): 
Input: 4
Output: 14
Runtime for mpi_Scatter: 0.000015 seconds.


As predicted mpi_send/recv was the slowest program. Mpi_scatter was a close second with a runtime of 0.000005 seconds slower than that of the quickest program, mpi_broadcast.


Step  2:

Predictions:

Send/Rec: Slowest performer again.

Gather: Faster than send and rec as a more optimised version of it.

Reduce: Fastest, more efficient.


Send/Rec:
Input: 4
Output: 14
Runtime for mpi_Send and mpi_Recv: 0.000005 seconds.


Gather: 
Input: 4
Output: 14
Runtime for mpiI_Gather: 0.000007 seconds.


Reduce & SUM:
Input: 4
Output: 14
Runtime for mpi_Reduce: 0.000005 seconds.


Running these programs in fact gave an identical runtime for send and rec as for reduce and sum. This program made need to be reanalysed to see if this result is in fact accurate. With these results mpi_gather proved slower than even mi_send and recv. 


Step 3:

Input: 4
MPI_SUM output: 14
Custom SUM operation: 14
Runtime for predefined MPI_SUM: 0.000192 seconds.
Runtime for custom sum operation: 0.000002 seconds.

Both methods are run through the same program and their accuracy is dealt in the accurate result of the sum of the squares for input = 4 which is 0,1,2, and 3. The custom sum manages to improve upon the runtime compared to the predefined MPI_SUM. The more tailored custom sum is more efficient and quite quicker than the more generic predefined version.


