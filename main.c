#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include "timer.h"
#include "Lab3IO.h"

// Implementation of Algorithm 1 from Lab Manual
void GuassianElimination(double **U, int size){
    int i,j,k;
    double temp;

    for (k = 0; k < size - 1; ++k) {
        // Partial pivoting
        int max_row = k;
        double max_val = U[k][k];

        #pragma omp parallel for reduction(max:max_val)
        for (i = k + 1; i < size; ++i) {
            if (abs(U[i][k]) > abs(max_val)) {
                max_val = U[i][k];
                max_row = i;
            }
        }

        if (max_row != k) {
            // Swap rows
            #pragma omp parallel for private(temp)
            for (j = 0; j < size + 1; ++j) {
                temp = U[k][j];
                U[k][j] = U[max_row][j];
                U[max_row][j] = temp;
            }
        }

        // Elimination
        #pragma omp parallel for private(i, j, temp)
        for (i = k + 1; i < size; ++i) {
            temp = U[i][k] / U[k][k];
            for (j = k; j < size + 1; ++j) {
                U[i][j] -= temp * U[k][j];
            }
        }
    }

}
// Implementation of Algorithm 2 from Lab Manual
void JordanElimination(double **D, int size){
    int i, k;

    for (k = size - 1; k >= 0; --k) {
        // Normalize the current row
        #pragma omp parallel for
        for (i = 0; i < size + 1; ++i) {
            D[k][i] /= D[k][k];
        }

        // Elimination
        #pragma omp parallel for private(i)
        for (i = 0; i < k; ++i) {
            double factor = D[i][k];
            for (int j = k; j < size + 1; ++j) {
                D[i][j] -= factor * D[k][j];
            }
        }
    }
}

int main (int argc, char *argv[]){
    double **A;
    double start, end;
    double *x;
    int size;

    if (argc != 2){
        printf("Usage: %s <number of threads>\n", argv[0]);
        return 1;
    }

    // Initialize the number of threads for OMP to use
    int num_threads = atoi(argv[1]);
    omp_set_num_threads(num_threads);

    // Load input data
    Lab3LoadInput(&A, &size);

    x = CreateVec(size); // Allocate memory for the resulting vector

    GET_TIME(start);

    GuassianElimination(A, size);
    JordanElimination(A, size);

    // Take the resulting vector from the reduced matrix
    for (int i = 0; i< size; i++){
        x[i]= A[i][size];
    }

    GET_TIME(end);

    // Save output
    Lab3SaveOutput(x, size, end - start);

    // Free allocated memory
    DestroyMat(A, size);
    DestroyVec(x);
    return 0;
}