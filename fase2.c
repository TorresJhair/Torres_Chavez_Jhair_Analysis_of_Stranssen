#define _POSIX_C_SOURCE 199309L
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "interfaces/matrix.h"
#include "interfaces/mult_classic.h"
#include "interfaces/strassen_multiple_cutoff.h"
#include "interfaces/strassen_power_2.h"
#include "interfaces/benchmark.h"

#define SEED 42
#define MATRIX_SIZE 256
#define STEP 2
#define RUNS 15

static int compare_double(const void* a, const void* b) {
    double da = *(const double*)a;
    double db = *(const double*)b;
    if (da < db) return -1;
    if (da > db) return 1;
    return 0;
}

static double median(double* values, int n) {
    qsort(values, n, sizeof(double), compare_double);
    if (n % 2 == 0) {
        return (values[n/2 - 1] + values[n/2]) / 2.0;
    } else {
        return values[n/2];
    }
}

static double stddev(double* values, int n) {
    double mean = 0.0;
    for (int i = 0; i < n; i++) mean += values[i];
    mean /= n;
    double variance = 0.0;
    for (int i = 0; i < n; i++) {
        double diff = values[i] - mean;
        variance += diff * diff;
    }
    variance /= n;
    return sqrt(variance);
}

int main() {

    srand(SEED);

    printf("# n classic_med classic_std multiple_cutoff_med multiple_cutoff_std power_of_2_med power_of_2_std (mediana y desv. est. de %d ejecuciones)\n", RUNS);

    for (int n = 2; n <= MATRIX_SIZE; n += STEP) {
        double* A = allocMatrix(n);
        double* B = allocMatrix(n);
        double* C_classic = allocMatrix(n);
        double* C_multiple_cutoff = allocMatrix(n);
        double* C_power_of_2 = allocMatrix(n);

        if (A == NULL || B == NULL || C_classic == NULL || C_multiple_cutoff == NULL || C_power_of_2 == NULL) {
            fprintf(stderr, "Error: no se pudo reservar memoria para n=%d\n", n);
            return EXIT_FAILURE;
        }

        randomMatrix(A, n);
        randomMatrix(B, n);

        double timesClassic[RUNS], timesMultipleCutoff[RUNS], timesPowerOf2[RUNS];

        for (int r = 0; r < RUNS; r++) {
            timesClassic[r] = measureClassicMult(A, B, C_classic, n);
            timesMultipleCutoff[r] = measureStrassenMultipleCutoff(A, B, C_multiple_cutoff, n);
            timesPowerOf2[r] = measureStrassenPowerOf2(A, B, C_power_of_2, n);
        }

        double medClassic = median(timesClassic, RUNS);
        double medMultipleCutoff = median(timesMultipleCutoff, RUNS);
        double medPowerOf2 = median(timesPowerOf2, RUNS);

        double stdClassic = stddev(timesClassic, RUNS);
        double stdMultipleCutoff = stddev(timesMultipleCutoff, RUNS);
        double stdPowerOf2 = stddev(timesPowerOf2, RUNS);

        int areEquals = equalMatrix(C_classic, C_multiple_cutoff, n);
        printf("%d\t%.3f\t%.3f\t%.3f\t%.3f\t%.3f\t%.3f\t%d\n",
               n, medClassic, stdClassic, medMultipleCutoff, stdMultipleCutoff, medPowerOf2, stdPowerOf2, areEquals);

        freeMatrix(A);
        freeMatrix(B);
        freeMatrix(C_classic);
        freeMatrix(C_multiple_cutoff);
        freeMatrix(C_power_of_2);
    }

    return EXIT_SUCCESS;
}