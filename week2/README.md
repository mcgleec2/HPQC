## Instructions on running the programs:

# Return to the home directory, then call the gcc compiler and tell it where to find the code.

# cd ~
# gcc HPQC/week2/time_print.c -o bin/time_print

# Measure the runtime of the C and Python versions of the programs for the desired numerical input value. 

# time bin/time_print [NUM]	 // enter a numerical value for [NUM]
# time python3 HPQC/week2time_print.py [NUM]	# enter a numerical value for [NUM]

# For the time_print1.c and time_print1.py programs use the following command to open file where the output is printed to:

# cat time_print1_output.txt
# cat time_print1.py.txt


## Conclusions on relative runtimes of C and Python programs:

# This exercise saw the use of three C and 3 Python programs.
# The C programs were titled time_print.c; time_print1.c; and time_print2.c.
# The Python programs were titled time_print.py; time_print1.py; and time_print2.py.
# Both time_print.c and time_print.py were copied from the programs of the same name within the PHY1090/week2 directory.
# Time_print1.c and time_print1.py both involve modifying the code of time_print.c and time_print.py, respectively, in order to have the program print the output to a file instead of the screen.
# For time_print1.c the output file was titled “time-print1_output.txt” and for the time_print1,py programs, “time_print1.py.txt”.
# Time_print2.c and time_print2.py involved further modifying the previous programs in order to read the output printed to the files into memory and print this to the screen: effectively giving the same result as the time_print.c and time_print.py programs but with vastly different runtimes.
# The inputs used for testing these programs were the numerical values 3, 30, and 3000. 
# For all programs the runtime remained very similar or increased as the numbers themselves increased; e.g. For C: 3 = 0.000134s; 30 = 0.000136s; and 3000 = 0.00788s.


# For the C programs a clear trend developed across the input values where printing outputs straight to the screen was far slower than printing to a file. 
# For example, for the input of 3: time_print.c = 0.000134s; time_print1.c = 0..000026s; and time_print2.c = 0.000048s.
# This shows that, even when reading the output from the file it was printed to, printing to the screen is the slowest method.
# The slight added time to time_print2.c is most likely accounted for by the extra step of reading to output from the file to memory and printing this read value to screen.
# This shows that to improve the efficiency of C programs in outputting a numerical value the output should be printed to a file and read from there as opposed to directly to the screen.
# Similar trends were seen for the 30 and 3000 input values with the major differences being increased runtimes caused by larger input values.

# For the Python programs an opposite trend was apparent. 
# Runtime increases considerably when printing the outputs to a file instead of printing directly to the screen.
# For example, for the input of 3: time_print.py = 1.36E-05s; time_print1.py = 0.002601147s; and time_print2.py = 0.003310442s. 
# As with C, there is a slight increase in runtime for time_print2.py as it contains the extra step of reading the output from the file and printing this read value to the screen.
# This shows that when printing a numerical value directly to the screen Python is a more efficient method.
#  Similar trends were seen for the 30 and 3000 input values with the major differences being increased runtimes caused by larger input values.
