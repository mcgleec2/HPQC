import sys
import time

def main():
    output = 0
    # checks if there are the right number of arguments
    try:
        # converts the first two arguments to integers
        in_arg = int(sys.argv[1])
    except: # (argc != 2)
        # raises an error
        raise Exception("Incorrect arguments.\nUsage: python time_print.py [NUM]\ne.g.\npython time_print.py 23")

    # gets the start time for the loop
    start_time = time.time()

    # open the file in write mode
    with open("data/week2/time_print1.py.txt", "w") as file:
    	# iterates over all numbers up to the input
    	for i in range(in_arg):
        	# writes the index to the file
        	file.write("{}, ".format(i))

    	# gets the end time for the loop
    	end_time = time.time()
    	# gets the total time
    	run_time = end_time - start_time

    	# writes the output to the file
    	file.write("\n\nTime for loop: {} seconds\n".format(run_time))

if __name__ == "__main__":
    main()
