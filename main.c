#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/*
gcc -Wall -O2 main.c -o main && /usr/bin/time -v ./main
or
gcc -Wall -g -O2 -DSMALL=1 main.c -o main && valgrind ./main
*/

// https://stackoverflow.com/a/35117084
double get_random()
{
    return ((double)rand() / (double)RAND_MAX);
}

struct Matrix
{
    int width;
    int height;
    double **data;
};

typedef struct Matrix Matrix;

Matrix create_random_matrix(int width, int height)
{
    double *memory = malloc(width * height * sizeof(double));

    double **data = NULL;
    data = malloc(height * sizeof(double *));

    for (size_t i = 0; i < height; i++)
    {
        data[i] = &memory[i * width];
    }

    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            data[y][x] = get_random();
        }
    }

    return (Matrix){.width = width,
                    .height = height,
                    .data = data};
}

void free_matrix(Matrix to_be_freed)
{
    free(to_be_freed.data[0]);
    free(to_be_freed.data);
}

Matrix matrix_multiplication(Matrix a, Matrix b)
{
    if (a.width != b.height)
    {
        printf("WRONG MATRIX SIZE, A WIDTH != B HEIGHT");
        exit(1);
    }

    Matrix result = create_random_matrix(b.width, a.height);

    for (int a_row = 0; a_row < a.height; a_row++)
    {
        for (int b_column = 0; b_column < b.width; b_column++)
        {
            double total = 0;
            for (int a_column_b_row = 0; a_column_b_row < a.width; a_column_b_row++)
            {
                total += a.data[a_row][a_column_b_row] * b.data[a_column_b_row][b_column];
            }

            result.data[a_row][b_column] = total;
            total = 0;
        }
    }

    return result;
}

void write_matrix_to_file(Matrix matrix, const char *filename)
{
    FILE *dest_file = fopen(filename, "wb");

    printf("Writing %ld x %d bytes to %s\n", sizeof(**matrix.data), matrix.height * matrix.width, filename);

    int ret = fwrite(*matrix.data, sizeof(**matrix.data), matrix.height * matrix.width, dest_file);

    if (ret != matrix.height * matrix.width)
    {
        printf("Stream error indication %d\n", ferror(dest_file));
        perror("");
    }

    fclose(dest_file);
}

int main()
{
    srand(time(NULL));

    Matrix b = create_random_matrix(1000000, 1000);
    write_matrix_to_file(b, "b.matrix");
    Matrix c = create_random_matrix(1, 1000000);
    write_matrix_to_file(c, "c.matrix");

    Matrix bc = matrix_multiplication(b, c);
    write_matrix_to_file(bc, "bc.matrix");

    free_matrix(b);
    free_matrix(c);

    Matrix a = create_random_matrix(1000, 1000000);
    write_matrix_to_file(a, "a.matrix");
    Matrix d = matrix_multiplication(a, bc);
    write_matrix_to_file(d, "d.matrix");

    free_matrix(a);
    free_matrix(bc);
    free_matrix(d);

    return 0;
}
