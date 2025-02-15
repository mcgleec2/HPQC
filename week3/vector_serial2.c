// Further modified version of vector_serial.c program from PHY1090
// Program now multiplies the vector elements together
// (which are already the squares of their indices)
// Zeroth element is skipped as: 0 * x = 0

#include <stdio.h>
#include <stdlib.h>

// declares the functions that will be called within main
int check_args(int argc, char **argv);
void initialise_vector(int vector[], int size);
void print_vector(int vector[], int size);
int multiply_vector(int vector[], int size);

int main(int argc, char **argv)
{
        // declare and initialise the numerical argument variable
        int num_arg = check_args(argc, argv);

        // creates a vector variable
        int* my_vector = malloc(num_arg * sizeof(int));

        // initialise the vector with squares of indices
        initialise_vector(my_vector, num_arg);

        // multiplies the vector values, skipping the 0 term
        int my_product = multiply_vector(my_vector, num_arg);

        // prints the product
        printf("Product: %d\n", my_product);

        // if we use malloc, must free when done!
        free(my_vector);

        return 0;
}

// defines a function to multiply a vector of ints into a single int
int multiply_vector(int vector[], int size)
{
        // create a variable to hold the product, starting at 1 (0^2 = 0 & 0 * x = 0)
        int product = 1;

        // check if the zeroth term is zero and skip it
        int start_index = (vector[0] == 0) ? 1 : 0;

        // iterates through the vector
        for (int i = start_index; i < size; i++)
        {
                // multiplies the current product with each vector element
                product *= vector[i];
        }

        // returns the product
        return product;
}

// defines a function to initialise all values in a vector to the square of their indices
void initialise_vector(int vector[], int size)
{
        // iterates through the vector
        for (int i = 0; i < size; i++)
        {
                // sets the elements of the vector to the square of the index
                vector[i] = i * i;
        }
}

// defines a function to print a vector of ints
void print_vector(int vector[], int size)
{
        // iterates through the vector
        for (int i = 0; i < size; i++)
        {
                // prints the elements of the vector to the screen
                printf("%d\n", vector[i]);
        }
}

// defines a function that checks your arguments to make sure they'll do what you need
int check_args(int argc, char **argv)
{
        // declare and initialise the numerical argument
        int num_arg = 0;

        // check the number of arguments
        if (argc == 2) // program name and numerical argument
        {
                // declare and initialise the numerical argument
                num_arg = atoi(argv[1]);
        }
        else // the number of arguments is incorrect
        {
                // raise an error
                fprintf(stderr, "ERROR: You did not provide a numerical argument!\n");
                fprintf(stderr, "Correct use: %s [NUMBER]\n", argv[0]);

                // and exit COMPLETELY
                exit(-1);
        }
        return num_arg;
}

