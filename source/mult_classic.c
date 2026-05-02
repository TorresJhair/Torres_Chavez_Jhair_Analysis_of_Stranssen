#include "../interfaces/mult_classic.h"

void mult_classic(int** A, int** B, int** C, int n) {
    // Matrix C, completamente nueva, rellena de 0's
    zero_matrix(C, n);
    for (int i = 0; i < n; i++)
        for (int k = 0; k < n; k++)
            for (int j = 0; j < n; j++)
                C[i][j] += A[i][k] * B[k][j];
}