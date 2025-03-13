#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <math.h>
#include "timer.h"
#include "Lab3IO.h"

double **A, *x;
int n;

/* Outputing the result and run time to 2 different files. Used for result verification and run time comparision 
 *
 * Para: fout -> File name for the output
 *       tout -> File name for the time measurement
 *       time -> End time result
 */
void PrintDebug(char *fout, char *tout, double time, int presult) {
    FILE *op, *t;
    int i;

    if ((op = fopen(fout,"w")) == NULL){
        printf("Error opening the output file.\n");
        exit(1);
    }
    if ((t = fopen(tout,"a")) == NULL){
        printf("Error opening the time output file.\n");
        exit(1);
    }

    if (presult) {
        fprintf(op, "%d\n", n);
        for (i = 0; i < n; ++i)
            fprintf(op, "%e\t", x[i]);
    }
    fprintf(t, "%lf\n", time);
    fclose(op);
    fclose(t);
}

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

    // Load input data
    Lab3LoadInput(&A, &n);

    x = CreateVec(n); // Allocate memory for the resulting vector

    GET_TIME(start);

    GuassianElimination(A);
    JordanElimination(A);
    
    //Take the resulting vector from the reduced matrix
    for (int i = 0; i < n; i++)
        x[i] = A[i][n] / A[i][i];
    
    GET_TIME(end);

    PrintDebug("serial_out.txt", "serial_time_out.txt", end-start, 1);

    // Free allocated memory
    DestroyMat(A, n);
    DestroyVec(x);
    return 0;
}