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

