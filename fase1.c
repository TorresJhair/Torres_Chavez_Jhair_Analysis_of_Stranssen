#define _POSIX_C_SOURCE 199309L
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "interfaces/matrix.h"
#include "interfaces/mult_classic.h"
#include "interfaces/strassen_mc32.h"

#define SEED 42
#define MATRIX_SIZE 512
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

static double measureClassic(double* A, double* B, double* C, int n) {
    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);
    mult_classic(A, B, C, n);
    clock_gettime(CLOCK_MONOTONIC, &end);
    long long ns = (end.tv_sec - start.tv_sec) * 1000000000LL + (end.tv_nsec - start.tv_nsec);
    return (double)ns / 1000.0;
}

static double measureMc32(double* A, double* B, double* C, int n) {
    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);
    strassen_mc32(A, B, C, n);
    clock_gettime(CLOCK_MONOTONIC, &end);
    long long ns = (end.tv_sec - start.tv_sec) * 1000000000LL + (end.tv_nsec - start.tv_nsec);
    return (double)ns / 1000.0;
}

int main() {

    srand(SEED);

    printf("# n classic_med classic_std mc32_med mc32_std (mediana y desv. est. de %d ejecuciones)\n", RUNS);

    for (int n = 2; n <= MATRIX_SIZE; n *= STEP) {
        double* A = allocMatrix(n);
        double* B = allocMatrix(n);
        double* C_classic = allocMatrix(n);
        double* C_mc32 = allocMatrix(n);

        if (A == NULL || B == NULL || C_classic == NULL || C_mc32 == NULL) {
            fprintf(stderr, "Error: no se pudo reservar memoria para n=%d\n", n);
            return EXIT_FAILURE;
        }

        randomMatrix(A, n);
        randomMatrix(B, n);

        double timesClassic[RUNS], timesMc32[RUNS];

        for (int r = 0; r < RUNS; r++) {
            timesClassic[r] = measureClassic(A, B, C_classic, n);
            timesMc32[r] = measureMc32(A, B, C_mc32, n);
        }

        double medClassic = median(timesClassic, RUNS);
        double medMc32 = median(timesMc32, RUNS);

        double stdClassic = stddev(timesClassic, RUNS);
        double stdMc32 = stddev(timesMc32, RUNS);

        int areEquals = equalMatrix(C_classic, C_mc32, n);
        printf("%d\t%.3f\t%.3f\t%.3f\t%.3f\t%d\n",
               n, medClassic, stdClassic, medMc32, stdMc32, areEquals);

        freeMatrix(A);
        freeMatrix(B);
        freeMatrix(C_classic);
        freeMatrix(C_mc32);
    }

    return EXIT_SUCCESS;
}