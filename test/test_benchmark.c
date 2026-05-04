#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "../interfaces/benchmark.h"
#include "../interfaces/matrix.h"
#include "../interfaces/mult_classic.h"
#include "../interfaces/strassen.h"

void test_elapsed_nanoseconds_zero() {
    struct timespec t;
    t.tv_sec = 100;
    t.tv_nsec = 500000000;

    long long diff = elapsedNanoseconds(t, t);
    assert(diff == 0);
}

void test_elapsed_nanoseconds_same_second() {
    struct timespec start = {100, 200000000};
    struct timespec end   = {100, 700000000};

    long long diff = elapsedNanoseconds(start, end);
    assert(diff == 500000000);
}

void test_elapsed_nanoseconds_cross_second() {
    struct timespec start = {100, 800000000};
    struct timespec end   = {101, 300000000};

    long long diff = elapsedNanoseconds(start, end);
    // 1 segundo - 500ms = 500ms
    assert(diff == 500000000);
}

void test_elapsed_nanoseconds_exact_second() {
    struct timespec start = {100, 0};
    struct timespec end   = {105, 0};

    long long diff = elapsedNanoseconds(start, end);
    assert(diff == 5000000000LL);
}

void test_measure_classic_returns_positive() {
    int n = 4;
    double** A = allocMatrix(n);
    double** B = allocMatrix(n);
    double** C = allocMatrix(n);

    srand(1);
    randomMatrix(A, n);
    randomMatrix(B, n);

    double time = measureClassicMult(A, B, C, n);
    assert(time >= 0.0);

    freeMatrix(A, n);
    freeMatrix(B, n);
    freeMatrix(C, n);
}

void test_measure_strassen_returns_positive() {
    int n = 4;
    double** A = allocMatrix(n);
    double** B = allocMatrix(n);
    double** C = allocMatrix(n);

    srand(2);
    randomMatrix(A, n);
    randomMatrix(B, n);

    double time = measureStrassen(A, B, C, n);
    assert(time >= 0.0);

    freeMatrix(A, n);
    freeMatrix(B, n);
    freeMatrix(C, n);
}

int test_main_benchmark() {
    printf("Tests Benchmark\n");

    test_elapsed_nanoseconds_zero();
    test_elapsed_nanoseconds_same_second();
    test_elapsed_nanoseconds_cross_second();
    test_elapsed_nanoseconds_exact_second();
    test_measure_classic_returns_positive();
    test_measure_strassen_returns_positive();

    printf("Todos los Tests de benchmark exitosos =)\n");
    return 0;
}
