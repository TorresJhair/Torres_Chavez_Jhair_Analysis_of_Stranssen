#include <stdlib.h>
#include <string.h>
#include "../interfaces/matrix.h"
#include <math.h>

double** allocMatrix(int n) {
    double** M = malloc(n * sizeof(double*));
    for (int i = 0; i < n; i++)
        M[i] = malloc(n * sizeof(double));
    return M;
}

void freeMatrix(double** M, int n) {
    for (int i = 0; i < n; i++)
        free(M[i]);
    free(M);
}

void copyMatrix(double** A, double** B, int n) {
    for (int i = 0; i < n; i++)
        memcpy(B[i], A[i], n * sizeof(double));
}

void randomMatrix(double** A, int n) {
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            A[i][j] = rand() % 10;
}

void zeroMatrix(double** M, int n) {
    for (int i = 0; i < n; i++)
        memset(M[i], 0, n * sizeof(double));
}

int equalMatrix(double** A, double** B, int n) {
    double EPS =  1e-9;

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {

            double a = A[i][j];
            double b = B[i][j];
            double diff = fabs(a - b);

            double scale = fmax(fabs(a), fabs(b));

            if (diff > EPS * (1.0 + scale)) {
                return 0;
            }
        }
    }
    return 1;
}

void addMatrix(double** A, double** B, double** C, int n) {
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            C[i][j] = A[i][j] + B[i][j];
}

void subMatrix(double** A, double** B, double** C, int n) {
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            C[i][j] = A[i][j] - B[i][j];
}