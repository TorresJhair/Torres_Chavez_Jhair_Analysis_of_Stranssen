#include <stdlib.h>
#include <string.h>
#include "../interfaces/matrix.h"
#include <math.h>

double* allocMatrix(int n) {
    return malloc(n * n * sizeof(double));
}

void freeMatrix(double* M) {
    free(M);
}

void copyMatrix(double* A, double* B, int n) {
    memcpy(B, A, n * n * sizeof(double));
}

void randomMatrix(double* A, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            // (rand() / RAND_MAX) genera un número entre 0 y 1
            double r = (double)rand() / (double)RAND_MAX;
            A[i * n + j] = r * 10.0; 
        }
    }
}

void zeroMatrix(double* M, int n) {
    memset(M, 0, n * n * sizeof(double));
}

int equalMatrix(double* A, double* B, int n) {
    double EPS = 1e-9;

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            double a = A[i * n + j];
            double b = B[i * n + j];
            double diff = fabs(a - b);
            double scale = fmax(fabs(a), fabs(b));

            if (diff > EPS * (1.0 + scale)) {
                return 0;
            }
        }
    }
    return 1;
}

void addMatrix(double* A, double* B, double* C, int n) {
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            C[i * n + j] = A[i * n + j] + B[i * n + j];
}

void subMatrix(double* A, double* B, double* C, int n) {
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            C[i * n + j] = A[i * n + j] - B[i * n + j];
}