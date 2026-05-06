#define _POSIX_C_SOURCE 199309L
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "interfaces/matrix.h"
#include "interfaces/mult_classic.h"
#include "interfaces/strassen_strided.h"
#include "interfaces/benchmark.h"

#define SEED 42
#define MATRIX_SIZE 2048
#define STEP 2
#define RUNS 10

int main() {

    srand(SEED);

    printf("# n classic_us strassen_strided_us (promedio de %d ejecuciones)\n", RUNS);

    for (int n = 2; n <= MATRIX_SIZE; n *= STEP) {
        double* A = allocMatrix(n);
        double* B = allocMatrix(n);
        double* C_classic = allocMatrix(n);
        double* C_strassen_strided = allocMatrix(n);

        if (A == NULL || B == NULL || C_classic == NULL || C_strassen_strided == NULL) {
            fprintf(stderr, "Error: no se pudo reservar memoria para n=%d\n", n);
            return EXIT_FAILURE;
        }

        randomMatrix(A, n);
        randomMatrix(B, n);

        double totalClassic = 0, totalStrassenStrided = 0;

        for (int r = 0; r < RUNS; r++) {
            totalClassic += measureClassicMult(A, B, C_classic, n);
            totalStrassenStrided += measureStrassenStrided(A, B, C_strassen_strided, n);
        }

        double avgClassic = totalClassic / RUNS;
        double avgStrassenStrided = totalStrassenStrided / RUNS;

        printf("%d\t%.3f\t%.3f\n", n, avgClassic, avgStrassenStrided);

        freeMatrix(A);
        freeMatrix(B);
        freeMatrix(C_classic);
        freeMatrix(C_strassen_strided);
    }

    return EXIT_SUCCESS;
}