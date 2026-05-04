#define _POSIX_C_SOURCE 199309L
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "interfaces/matrix.h"
#include "interfaces/mult_classic.h"
#include "interfaces/strassen.h"
#include "interfaces/benchmark.h"

#define SEED 42
#define MATRIX_SIZE 64
#define STEP 2

int main() {

    srand(SEED);

    printf("# n classic_us strassen_us\n");

    for (int n = 2; n <= MATRIX_SIZE; n += STEP) {
        double** A = allocMatrix(n);
        double** B = allocMatrix(n);
        double** C_classic = allocMatrix(n);
        double** C_strassen = allocMatrix(n);

        if (A == NULL || B == NULL || C_classic == NULL || C_strassen == NULL) {
            fprintf(stderr, "Error: no se pudo reservar memoria para n=%d\n", n);
            return EXIT_FAILURE;
        }

        randomMatrix(A, n);
        randomMatrix(B, n);

        double timeClassic = measureClassicMult(A, B, C_classic, n);
        double timeStrassen = measureStrassen(A, B, C_strassen, n);

        printf("%d\t%.3f\t%.3f\n", n, timeClassic, timeStrassen);

        freeMatrix(A, n);
        freeMatrix(B, n);
        freeMatrix(C_classic, n);
        freeMatrix(C_strassen, n);
    }

    return EXIT_SUCCESS;
}
