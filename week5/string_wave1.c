// Headers and Libraries
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Structure to hold the configuration
struct config {
    int cycles;
    int samples;
    int points;
    char output_file[100];
};

// Declares the functions that will be called within main
int check_args(int argc, char **argv, struct config* cfg);
void initialise_vector(double vector[], int size, double initial);
void print_vector(double vector[], int size);
int sum_vector(int vector[], int size);
void update_positions(double* positions, int points, double time);
int generate_timestamps(double* time_stamps, int time_steps, double step_size);
double driver(double time);
void print_header(FILE* p_out_file, int points);

int main(int argc, char **argv)
{
    // Declares and initializes the configuration structure
    struct config cfg;

    // Split the arguments into the config struct
    check_args(argc, argv, &cfg);

    // Uses given values
    int time_steps = cfg.cycles * cfg.samples + 1; // total time steps
    double step_size = 1.0 / cfg.samples;

    // Creates a vector for the time stamps in the data
    double* time_stamps = (double*) malloc(time_steps * sizeof(double));
    initialise_vector(time_stamps, time_steps, 0.0);
    generate_timestamps(time_stamps, time_steps, step_size);

    // Creates a vector variable for the current positions
    double* positions = (double*) malloc(cfg.points * sizeof(double));
    initialise_vector(positions, cfg.points, 0.0);

    // Creates and opens the output file
    FILE* out_file = fopen(cfg.output_file, "w");
    if (out_file == NULL) {
        fprintf(stderr, "ERROR: Unable to open output file %s\n", cfg.output_file);
        return -1;
    }
    print_header(out_file, cfg.points);

    // Iterates through each time step in the collection
    for (int i = 0; i < time_steps; i++) {
        // Updates the positions using a function
        update_positions(positions, cfg.points, time_stamps[i]);

        // Prints an index and time stamp
        fprintf(out_file, "%d, %lf", i, time_stamps[i]);

        // Iterates over all of the points on the line
        for (int j = 0; j < cfg.points; j++) {
            // Prints each y-position to the file
            fprintf(out_file, ", %lf", positions[j]);
        }
        // Prints a new line
        fprintf(out_file, "\n");
    }

    // Frees malloc
    free(time_stamps);
    free(positions);

    // Closes the file
    fclose(out_file);

    return 0;
}

// Prints a header to the file
void print_header(FILE* p_out_file, int points)
{
    fprintf(p_out_file, "#, time");
    for (int j = 0; j < points; j++) {
        fprintf(p_out_file, ", y[%d]", j);
    }
    fprintf(p_out_file, "\n");
}

// Defines a simple harmonic oscillator as the driving force
double driver(double time)
{
    return sin(time * 2.0 * M_PI);
}

// Updates the positions of the string
void update_positions(double* positions, int points, double time)
{
    double* new_positions = (double*) malloc(points * sizeof(double));

    // Sets the first position to the driver value
    new_positions[0] = driver(time);

    // Propagates the positions along the string
    for (int i = 1; i < points; i++) {
        new_positions[i] = positions[i-1];
    }

    // Updates the positions to the new values
    for (int i = 0; i < points; i++) {
        positions[i] = new_positions[i];
    }

    // Frees memory
    free(new_positions);
}

// Generates the timestamps for the simulation
int generate_timestamps(double* timestamps, int time_steps, double step_size)
{
    for (int i = 0; i < time_steps; i++) {
        timestamps[i] = i * step_size;
    }
    return time_steps;
}

// Checks the command line arguments
int check_args(int argc, char **argv, struct config* cfg)
{
    // Error check for arguments
    if (argc != 5) {
        fprintf(stderr, "ERROR: Incorrect number of arguments!\n");
        fprintf(stderr, "Usage: %s <cycles> <samples> <points> <output_file>\n", argv[0]);
        exit(-1);
    }

    // Splits the arguments into the config struct
    cfg->cycles = atoi(argv[1]);
    cfg->samples = atoi(argv[2]);
    cfg->points = atoi(argv[3]);
    snprintf(cfg->output_file, sizeof(cfg->output_file), "%s", argv[4]);

    // Error check the input values
    if (cfg->cycles <= 0 || cfg->samples <= 0 || cfg->points <= 0) {
        fprintf(stderr, "ERROR: Cycles, samples, and points must be positive integers!\n");
        exit(-1);
    }

    return 0;
}

// Initializes a vector with a given initial value
void initialise_vector(double vector[], int size, double initial)
{
    for (int i = 0; i < size; i++) {
        vector[i] = initial;
    }
}

