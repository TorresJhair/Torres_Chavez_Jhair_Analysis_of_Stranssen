#include "../interfaces/matrix.h"
#include "../interfaces/mult_classic.h"

void mult_classic(double** A, double** B, double** C, int n) {
    // Matrix C, completamente nueva, rellena de 0's
    zeroMatrix(C, n);
    for (int i = 0; i < n; i++)
        for (int k = 0; k < n; k++)
            for (int j = 0; j < n; j++)
                C[i][j] += A[i][k] * B[k][j];
}