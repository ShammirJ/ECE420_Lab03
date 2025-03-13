/*
ECE420 Lab3: Solving a Linear System of Equations via Gauss-Jordan Elimination using OpenMP
Lab Section: H21 (Group06)
Members: Zaryab Ahmed, Adil Tapal, Samuel Fritz
*/

#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <math.h>
#include "timer.h"
#include "Lab3IO.h"

double start, end, Time;
/*
Below is the function that performs Gauss-Jordan elimination. The Gauss-Jordan elimination
logic was found in the lab manual, where it described the steps to perform the elimination.
The function takes in a 2D array A and an integer n, where n is the number of rows and columns
in the matrix A. The function first performs partial pivoting to find the maximum element in the
column, then swaps the rows if needed. The function then normalizes the pivot row by dividing the
row by the pivot. The function then eliminates other rows by subtracting the factor times the pivot
row from the row. The function uses OpenMP to parallelize the for loops to speed up the computation.
*/
void gauss_jordan_elimination(double **A, int n) {
    // Start time using timer.h
    GET_TIME(start);

    for (int k = 0; k < n; k++) {
        // Partial Pivoting: Find the maximum element in column k
        int max_row = k;
        for (int i = k + 1; i < n; i++) {
            if (fabs(A[i][k]) > fabs(A[max_row][k])) {
                max_row = i;
            }
        }
        
        // Swap rows if needed
        if (max_row != k) {
            double *temp = A[k];
            A[k] = A[max_row];
            A[max_row] = temp;
        }
        
        // Normalize pivot row
        double pivot = A[k][k];
        #pragma omp parallel for
        for (int j = k; j < n + 1; j++) {
            // divide the row by the pivot
            A[k][j] /= pivot;
        }
        
        // Eliminate other rows
        // Step zeros out of all non pivot elements in the column
        #pragma omp parallel for
        for (int i = 0; i < n; i++) {
            if (i != k) {
                double factor = A[i][k];
                for (int j = k; j < n + 1; j++) {
                    // update the row by subtracting the factor times the pivot row
                    A[i][j] -= factor * A[k][j];
                }
            }
        }
    }

    // End time using timer.h
    GET_TIME(end);
    
}

int main(int argc, char *argv[]) {
    // Default number of threads
    int num_threads = 10;
    
    // Check if number of threads is provided as command line argument
    if (argc > 1) {
        num_threads = strtol(argv[1], NULL, 10);
        // Error handling for invalid input
        if (num_threads <= 0) {
            printf("Invalid number of threads. Using default (10).\n");
            num_threads = 10;
        }
    } else {
        printf("Number of threads not specified. Using default (10).\n");
    }
    
    // Set the number of threads
    omp_set_num_threads(num_threads);
    //printf("Running with %d threads\n", num_threads);

    int n;
    double **A;
    double *x;
    
    // Read input matrix using Lab3IO
    Lab3LoadInput(&A, &n);
    
    // Perform Gauss-Jordan elimination
    gauss_jordan_elimination(A, n);

    // Calculate the time
    Time = end - start;

    // Allocate memory for the result vector
    x = (double *)malloc(n * sizeof(double));
    for (int i = 0; i < n; i++) {
        x[i] = A[i][n];
    }

    // Save the results
    // Lab3SaveOutput(x, n, Time);

    FILE *op, *t;
    int i;

    if ((op = fopen("guess_output.txt","w")) == NULL){
        printf("Error opening the output file.\n");
        return 1;
    }
    if ((t = fopen("guess_time_output.txt","a")) == NULL){
        printf("Error opening the time output file.\n");
        return 1;
    }

    fprintf(op, "%d\n", n);
    for (i = 0; i < n; ++i)
        fprintf(op, "%e\t", x[i]);
    fprintf(t, "%lf\n", Time);
    fclose(op);
    fclose(t);
    
    // Free allocated memory
    for (int i = 0; i < n; i++) {
        free(A[i]);
    }
    free(A);
    free(x);
    
    return 0;
}