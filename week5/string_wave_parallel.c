#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>

// Function declarations
int check_args(int argc, char **argv);
void initialise_vector(double vector[], int size, double initial);
void print_vector(double vector[], int size);
int sum_vector(int vector[], int size);
void update_positions(double* positions, int local_points, double time, int rank);
int generate_timestamps(double* time_stamps, int time_steps, double step_size);
double driver(double time);
void print_header(FILE** p_out_file, int points);

int main(int argc, char **argv)
{
    // Start MPI
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); // This process's rank (ID)
    MPI_Comm_size(MPI_COMM_WORLD, &size); // Total number of processes

    int points = check_args(argc, argv);

    // Each process handles a chunk of the full string
    if (points % size != 0) {
        if (rank == 0) {
            fprintf(stderr, "Run with number of points divisble by num processes.\n");
        }
        MPI_Finalize();
        exit(1);
    }

    int local_points = points / size;

    // Timing setup
    int cycles = 5;
    int samples = 25;
    int time_steps = cycles * samples + 1;
    double step_size = 1.0 / samples;

    double* time_stamps = (double*) malloc(time_steps * sizeof(double));
    initialise_vector(time_stamps, time_steps, 0.0);
    generate_timestamps(time_stamps, time_steps, step_size);

    // Each process handles part of the string
    double* local_positions = (double*) malloc(local_points * sizeof(double));
    initialise_vector(local_positions, local_points, 0.0);

    // Only the master will write to file and collect data
    FILE* out_file = NULL;
    if (rank == 0) {
        out_file = fopen("data/string_wave.csv", "w");
        print_header(&out_file, points);
    }

    // Buffer for gathering all positions at master
    double* gathered_positions = NULL;
    if (rank == 0) {
        gathered_positions = (double*) malloc(points * sizeof(double));
    }

    for (int t = 0; t < time_steps; t++) {
        update_positions(local_positions, local_points, time_stamps[t], rank);

        // Gather all local positions to the master
        MPI_Gather(local_positions, local_points, MPI_DOUBLE,
                   gathered_positions, local_points, MPI_DOUBLE,
                   0, MPI_COMM_WORLD);

        // Master writes to file
        if (rank == 0) {
            fprintf(out_file, "%d, %lf", t, time_stamps[t]);
            for (int j = 0; j < points; j++) {
                fprintf(out_file, ", %lf", gathered_positions[j]);
            }
            fprintf(out_file, "\n");
        }
    }

    // Clean-up
    if (rank == 0) {
        fclose(out_file);
        free(gathered_positions);
    }
    free(local_positions);
    free(time_stamps);

    // Finalise MPI
    MPI_Finalize();

    return 0;
}

// Simple wave driver
double driver(double time)
{
    return sin(time * 2.0 * M_PI);
}

// Update function
void update_positions(double* positions, int local_points, double time, int rank)
{
    double* new_positions = (double*) malloc(local_points * sizeof(double));

    for (int i = 0; i < local_points; i++) {
        if (rank == 0 && i == 0) {
            // Leftmost process and first point - apply the driver
            new_positions[i] = driver(time);
        } else if (i == 0) {
            // For all other processes, request the previous value from neighbour
            double left_value;
            MPI_Recv(&left_value, 1, MPI_DOUBLE, rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            new_positions[i] = left_value;
        } else {
            new_positions[i] = positions[i - 1];
        }
    }

    // Send last value to the right neighbour if not last process
    if (rank < MPI_COMM_WORLD - 1) {
        MPI_Send(&positions[local_points - 1], 1, MPI_DOUBLE, rank + 1, 0, MPI_COMM_WORLD);
    }

    for (int i = 0; i < local_points; i++) {
        positions[i] = new_positions[i];
    }

    free(new_positions);
}

// defines a set of timestamps
int generate_timestamps(double* timestamps, int time_steps, double step_size)
{
    for (int i = 0; i < time_steps ; i++) {
        timestamps[i] = i * step_size;
    }
    return time_steps;
}

// Vector sum (not used in parallel version, but kept for completeness)
int sum_vector(int vector[], int size)
{
    int sum = 0;
    for (int i = 0; i < size; i++) {
        sum += vector[i];
    }
    return sum;
}

// Initialises vector values
void initialise_vector(double vector[], int size, double initial)
{
    for (int i = 0; i < size; i++) {
        vector[i] = initial;
    }
}

// defines a functin to print a vector of ints
void print_vector(double vector[], int size)
{
    for (int i = 0; i < size; i++) {
        printf("%d, %lf\n", i, vector[i]);
    }
}

// defines a function that checks your arguments to make sure they'll do what you need
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

// Print CSV header
void print_header(FILE** p_out_file, int points)
{
    fprintf(*p_out_file, "#, time");
    for (int j = 0; j < points; j++) {
        fprintf(*p_out_file, ", y[%d]", j);
    }
    fprintf(*p_out_file, "\n");
}
