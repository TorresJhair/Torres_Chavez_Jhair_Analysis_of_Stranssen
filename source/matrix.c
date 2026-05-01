#include <stdlib.h>
#include <string.h>
#include "../interfaces/matrix.h"

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