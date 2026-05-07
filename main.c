#define _POSIX_C_SOURCE 199309L
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "interfaces/matrix.h"
#include "interfaces/mult_classic.h"
#include "interfaces/strassen_multiple_cutoff.h"
#include "interfaces/strassen_power_2.h"
#include "interfaces/benchmark.h"

#define SEED 42
#define MATRIX_SIZE 256
#define STEP 2
#define RUNS 15

int main() {

    srand(SEED);

    printf("# n classic_us strided_us strided_64_us (promedio de %d ejecuciones)\n", RUNS);

    for (int n = 2; n <= MATRIX_SIZE; n += STEP) {
        double* A = allocMatrix(n);
        double* B = allocMatrix(n);
        double* C_classic = allocMatrix(n);
        double* C_strided = allocMatrix(n);
        double* C_strided_64 = allocMatrix(n);

        if (A == NULL || B == NULL || C_classic == NULL || C_strided == NULL || C_strided_64 == NULL) {
            fprintf(stderr, "Error: no se pudo reservar memoria para n=%d\n", n);
            return EXIT_FAILURE;
        }

        randomMatrix(A, n);
        randomMatrix(B, n);

        double totalClassic = 0, totalStrided = 0, totalStrided64 = 0;

        for (int r = 0; r < RUNS; r++) {
            totalClassic += measureClassicMult(A, B, C_classic, n);
            totalStrided += measureStrassenMultipleCutoff(A, B, C_strided, n);
            totalStrided64 += measureStrassenPowerOf2(A, B, C_strided_64, n);
        }

        double avgClassic = totalClassic / RUNS;
        double avgStrided = totalStrided / RUNS;
        double avgStrided64 = totalStrided64 / RUNS;

        int areEquals = equalMatrix(C_classic, C_strided, n);
        printf("%d\t%.3f\t%.3f\t%.3f\t%d\n", n, avgClassic, avgStrided, avgStrided64, areEquals);

        freeMatrix(A);
        freeMatrix(B);
        freeMatrix(C_classic);
        freeMatrix(C_strided);
        freeMatrix(C_strided_64);
    }

    return EXIT_SUCCESS;
}