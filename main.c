/* IMPORTANT: We have tried to parallelize Gaussian and Jordan elimination before, then we realized that parallel directives nested in the inner loop actually would slow the program down
 * due to thread creation overhead. Therefore, we decided that it would be best if Gaussian and Jordan elimination be executed in serial and only the updating to x part need to be
 * parallelized
 * 
 * Author: Dang Nguyen, Jose Shammir
 * Run as: ./main num_thread
 */

#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <math.h>
#include "timer.h"
#include "Lab3IO.h"

#define DEBUG_MODE 0 // Enable/Disable Debug mode. Set this to 1 first to use either the .sh program for testing

double **A, *x;
int n;

/* Outputing the result and run time to 2 different files. Used for result verification and run time comparision 
 *
 * Para: fout -> File name for the output
 *       tout -> File name for the time measurement
 *       time -> End time result
 *       presult -> If 0, don't print the result to save time. Used for run time comparision
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

/* Perform Gaussian Elimination using Algorithm 1 from Lab Manual
 * 
 * Para: G -> The intial G matrix = {A|b}
 */
void GuassianElimination(double **G) {
    double temp;

    for (int k = 0; k < n - 1; k++) {
        
        // Partial pivoting
        int max_row = k;
        double max_val = fabs(G[k][k]);

        for (int i = k + 1; i < n; i++) {
            if ((temp = fabs(G[i][k])) > max_val) {
                max_val = temp;
                max_row = i;
            }
        }

        if (max_row != k) {
            for (int j = 0; j < n + 1; j++) {
                temp = G[k][j];
                G[k][j] = G[max_row][j];
                G[max_row][j] = temp;
            }
        }

        // Elimination
        #pragma omp for private(temp)
        for (int i = k + 1; i < n; i++) {
            temp = G[i][k] / G[k][k];
            for (int j = k; j < n + 1; j++) {
                G[i][j] -= temp * G[k][j];
            }
        }

    }
}

/* Perform Jordan Elimination using Algorithm 2 from Lab Manual
 *
 * Para: U -> The output matrix from the Gaussian Elimination
 */
void JordanElimination(double **U) {
    /* Multiple trials and errors show that Jordan elimination is best computed in series */
    for (int k = n - 1; k > 0; k--) {
        for (int i = 0; i < k; i++) {
            U[i][n] -= (U[i][k] / U[k][k]) * U[k][n];
            U[i][k] = 0;
        }
    }
}


int main (int argc, char *argv[]){
    double start, end;

    if (argc != 2) {
        printf("Usage: %s <number of threads>\n", argv[0]);
        return 1;
    }

    // Initialize the number of threads for OMP to use
    int num_threads = atoi(argv[1]);
    omp_set_num_threads(num_threads);

    // Load input data
    Lab3LoadInput(&A, &n);

    x = CreateVec(n); // Allocate memory for the resulting vector

    GET_TIME(start);

    GuassianElimination(A);
    JordanElimination(A);
    
    // Take the resulting vector from the reduced matrix
    #pragma omp parallel for
    for (int i = 0; i < n; i++)
        x[i] = A[i][n] / A[i][i];
    
    GET_TIME(end);

    // Save output
    Lab3SaveOutput(x, n, end - start);

    if (DEBUG_MODE == 1) PrintDebug("main_out.txt", "main_time_out.txt", end-start, 1);

    // Free allocated memory
    DestroyMat(A, n);
    DestroyVec(x);
    return 0;
}