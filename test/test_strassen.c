#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>

#include "../interfaces/strassen.h"
#include "../interfaces/mult_classic.h"
#include "../interfaces/matrix.h"

void test_strassen_2x2() {
    int n = 2;

    double* A = allocMatrix(n);
    double* B = allocMatrix(n);
    double* C = allocMatrix(n);

    A[0*n + 0] = 1.0; A[0*n + 1] = 2.0;
    A[1*n + 0] = 3.0; A[1*n + 1] = 4.0;

    B[0*n + 0] = 5.0; B[0*n + 1] = 6.0;
    B[1*n + 0] = 7.0; B[1*n + 1] = 8.0;

    strassen(A, B, C, n);

    double* result = allocMatrix(n);
    result[0*n + 0] = 19.0;
    result[0*n + 1] = 22.0;
    result[1*n + 0] = 43.0;
    result[1*n + 1] = 50.0;

    assert(equalMatrix(result, C, n));

    freeMatrix(A);
    freeMatrix(B);
    freeMatrix(C);
    freeMatrix(result);
}

void test_strassen_4x4() {
    int n = 4;

    double* A = allocMatrix(n);
    double* B = allocMatrix(n);
    double* C_strassen = allocMatrix(n);
    double* C_classic = allocMatrix(n);

    srand(10);
    randomMatrix(A, n);
    randomMatrix(B, n);

    strassen(A, B, C_strassen, n);
    mult_classic(A, B, C_classic, n);

    assert(equalMatrix(C_strassen, C_classic, n));

    freeMatrix(A);
    freeMatrix(B);
    freeMatrix(C_strassen);
    freeMatrix(C_classic);
}

void test_strassen_identity() {
    int n = 4;

    double* A = allocMatrix(n);
    double* I = allocMatrix(n);
    double* C = allocMatrix(n);

    srand(20);
    randomMatrix(A, n);

    zeroMatrix(I, n);
    for (int i = 0; i < n; i++)
        I[i*n + i] = 1.0;

    strassen(A, I, C, n);

    assert(equalMatrix(A, C, n));

    freeMatrix(A);
    freeMatrix(I);
    freeMatrix(C);
}

void test_strassen_zero_matrix() {
    int n = 4;

    double* A = allocMatrix(n);
    double* Z = allocMatrix(n);
    double* C = allocMatrix(n);

    srand(30);
    randomMatrix(A, n);

    zeroMatrix(Z, n);

    strassen(A, Z, C, n);

    assert(equalMatrix(Z, C, n));

    freeMatrix(A);
    freeMatrix(Z);
    freeMatrix(C);
}

void test_strassen_5x5() {
    int n = 5;

    double* A = allocMatrix(n);
    double* B = allocMatrix(n);
    double* C_strassen = allocMatrix(n);
    double* C_classic = allocMatrix(n);

    srand(42);
    randomMatrix(A, n);
    randomMatrix(B, n);

    strassen(A, B, C_strassen, n);
    mult_classic(A, B, C_classic, n);

    assert(equalMatrix(C_strassen, C_classic, n));

    freeMatrix(A);
    freeMatrix(B);
    freeMatrix(C_strassen);
    freeMatrix(C_classic);
}

int test_main_strassen() {
    printf("Tests Strassen\n");

    test_strassen_2x2();
    test_strassen_4x4();
    test_strassen_identity();
    test_strassen_zero_matrix();
    test_strassen_5x5();

    printf("Todos los Tests de strassen exitosos =)\n");
    return 0;
}
