Practical 5 Programs & Exercises:

Part 1:

string_wave1.c
Run program: 
gcc HPQC/week5/string_wave1.c -o bin/week5/string_wave1 -lm
./bin/week5/string_wave1 10 100 50 ~/data/string_wave1.csv
arg[1] = cycles = 10
arg[2] = samples = 100
arg[3] = points = 50
arg[4] = output file & location = ~/data/string_wave1.csv

animate_line_file1.py
Run program:
python3 HPQC/week5/animate_line)file1.py data/string_wave1.csv  10 100 50 data/animate_line_file1.gif
arg[1] = input file = data/string_wave1.csv
arg[2] = cycles = 10
arg[3] = samples = 100
arg[4] = points = 50
arg[5] = output file = data/animate_line_file1.gif
display data/animate_string_file1.gif

For both programs the hard coded inputs for the number of cycles, the number of samples per cycle,
and the number of points along the string were made into command-line arguments. 
Along with this both programs give the output file as an argument too,
and for the animate_line_file1.py program the input file to be used is also an argument.

Part 2:
Step 1:
Patterns of Iteration:
- Outer loop: Time steps
- Inner loop: Points along the string
- Updating positions: Update positions func and position array

Sequential:
- Time step: Each iteration relies on the previous one
- Writing data to file: fprintf() wites results to file after each time step

Parallel: 
- Points along the string: Independently update each point position

Parallelisation:
- Splits the points into chunkc, each of which is dealt with by seperate processor
- Update each chunk of points for the current time step
- Each chunk will need to communicate with its neighbouring chunks
- Share boundary of one chunk with its neighbour
- Move on to next step and repeat process

Step 2: 
- Can't have multiple processes writing to the same file so,
- Need to combine reults before writing to file
- Parallel code so each process has its own chunk which need to be combined
- Hold the results in memory, combine them together, then write thefukk set to file
- Use MPI_Gather to combine MPI result to root process
- Write teh data to file and then perfoem benchmarking of code.

Step 3:
- Each process handles a section of the string
- Root process writes to the output file
- Each process will calculae it sown position then send to root process for each time step/

Step 4:
- Added in velocity and acceleration vectors
- used constants k and m and a step_size, dt
- Modified updaet_positions function to get the accerlation using spring force equation, then update velocity and position using Euler intergation method
- Added updaets to main to initialise and free velocity an dacceleration too.
