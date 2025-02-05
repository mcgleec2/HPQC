#include <stdio.h>
#include <stdlib.h>
#include <time.h>

double to_second_float(struct timespec in_time);
struct timespec calculate_runtime(struct timespec start_time, struct timespec end_time);

int main(int argc, char **argv)
{
        // creates and initialises the variables
        int i, input;
        i = input = 0;
        struct timespec start_time, end_time, time_diff;
        double runtime = 0.0;

        // checks if there are the right number of arguments
        if (argc == 2)
        {
                // converts the first argument to an integer
                input = atoi(argv[1]);
        }
        else //(argc != 2)
        {
                // raises an error
                fprintf(stderr, "Incorrect arguments.  Usage: time_print [NUM]\ne.g. \n time_print 3\n");
                // and crashes out
                exit(-1);
        }

        // open the output file for writing
        FILE *file = fopen("time_print1_output.txt", "w");
        if (file == NULL)
        {
                fprintf(stderr, "Failed to open output file. \n");
                exit(-1);
        }

        // gets the time before the loop
        timespec_get(&start_time, TIME_UTC);
        // iterates over all numbers up to the input
        for (i = 0; i < input; i++)
        {
                // writes the index to the file
                fprintf(file, "%d, ", i);
        }
        // gets the time after the loop
        timespec_get(&end_time, TIME_UTC);

        // calculates the runtime
        time_diff = calculate_runtime(start_time, end_time);
        runtime = to_second_float(time_diff);


        // writes the runtime to the file
        fprintf(file, "\n\nRuntime for core loop: %lf seconds.\n\n", runtime);

        // closes the file after writing
        fclose(file);

	// reads the contents of the file to memory
	char buffer[1024]; // change size as needed
	FILE *file_read = fopen("time_print1_output.txt", "r");
	if (file_read == NULL)
	{
		fprintf(stderr, "Failed to open file for reading. \n");
		exit(-1);
	}

	// reads and prints the contents of the file
	while (fgets(buffer, sizeof(buffer), file_read) != NULL)
	{
		// prints each line
		printf("%s", buffer);
	}

	// closes the file after reading
	fclose(file_read);

        return 0;
}


double to_second_float(struct timespec in_time)
{
        // creates and initialises the variables
        float out_time = 0.0;
        long int seconds, nanoseconds;
        seconds = nanoseconds = 0;

        // extracts the elements from in_time
        seconds = in_time.tv_sec;
        nanoseconds = in_time.tv_nsec;

        // calculates the time in seconds by adding the seconds and the nanoseconds divided by 1e9
        out_time = seconds + nanoseconds/1e9;

        // returns the time as a double
        return out_time;
}

struct timespec calculate_runtime(struct timespec start_time, struct timespec end_time)
{
        // creates and initialises the variables
        struct timespec time_diff;
        long int seconds, nanoseconds;                                                                                                       seconds = nanoseconds = 0;
        double runtime = 0.0;

        // extracts the elements from start_time and end_time
        seconds = end_time.tv_sec - start_time.tv_sec;
        nanoseconds = end_time.tv_nsec - start_time.tv_nsec;

        // if the ns part is negative
        if (nanoseconds < 0)
        {
                // "carry the one!"
                seconds = seconds - 1;
                nanoseconds = ((long int) 1e9) - nanoseconds;
        }

        // creates the runtime
        time_diff.tv_sec = seconds;
        time_diff.tv_nsec = nanoseconds;

        return time_diff;
}
