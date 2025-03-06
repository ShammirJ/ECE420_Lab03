#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <math.h>
#include "timer.h"
#include "Lab3IO.h"

double **A, *x;
int n;

void GuassianElimination(double **G) {
    double temp;

    for (int k = 0; k < n - 1; k++) {
        
        // Partial pivoting
        int max_row = k;
        double max_val = fabs(G[k][k]);

        {
            for (int i = k + 1; i < n; i++) {
                if ((temp = fabs(G[i][k])) > max_val) {
                    if (temp > max_val) {
                        max_val = temp;
                        max_row = i;
                    }
                }
            }

            if (max_row != k) {
                
                // Swap rows
                for (int j = 0; j < n + 1; j++) {
                    temp = G[k][j];
                    G[k][j] = G[max_row][j];
                    G[max_row][j] = temp;
                }
            }

            // Elimination
            for (int i = k + 1; i < n; i++) {
                temp = G[i][k] / G[k][k];
                for (int j = k; j < n + 1; j++) {
                    G[i][j] -= temp * G[k][j];
                }
            }
        }
    }
}

/* Perform Jordan Elimination using Algorithm 2 from Lab Manual
 *
 * Para: U -> The output matrix from the Gaussian Elimination
 */
void JordanElimination(double **U) {
    for (int k = n - 1; k > 0; k--) {
        for (int i = 0; i < k; i++) {
            U[i][n] -= (U[i][k] / U[k][k]) * U[k][n];
            U[i][k] = 0;
        }
    }
}

int main (){
    double start, end;

    // if (argc != 2) {
    //     printf("Usage: %s <number of threads>\n", argv[0]);
    //     return 1;
    // }

    // // Initialize the number of threads for OMP to use
    // int num_threads = atoi(argv[1]);
    // omp_set_num_threads(num_threads);

    // Load input data
    Lab3LoadInput(&A, &n);

    x = CreateVec(n); // Allocate memory for the resulting vector

    GET_TIME(start);

    GuassianElimination(A);
    PrintMat(A, n, n+1); // For debug
    JordanElimination(A);
    printf("\n\n");
    PrintMat(A, n, n+1); // For debug
    
    //Take the resulting vector from the reduced matrix
    for (int i = 0; i < n; i++)
        x[i] = A[i][n] / A[i][i];
    
    GET_TIME(end);

    // Save output
    Lab3SaveOutput(x, n, end - start);

    // Free allocated memory
    DestroyMat(A, n);
    DestroyVec(x);
    return 0;
}