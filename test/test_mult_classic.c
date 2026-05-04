#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>

#include "../interfaces/mult_classic.h"
#include "../interfaces/matrix.h"

// -----------------------------
// Test 1: Caso conocido 2x2
// -----------------------------
void test_multiplicacion_2x2() {
    int n = 2;

    double* A = allocMatrix(n);
    double* B = allocMatrix(n);
    double* C = allocMatrix(n);

    // A = [1 2; 3 4]
    A[0 * n + 0] = 1.0; A[0 * n + 1] = 2.0;
    A[1 * n + 0] = 3.0; A[1 * n + 1] = 4.0;

    // B = [5 6; 7 8]
    B[0 * n + 0] = 5.0; B[0 * n + 1] = 6.0;
    B[1 * n + 0] = 7.0; B[1 * n + 1] = 8.0;

    mult_classic(A, B, C, n);

    double* result = allocMatrix(n);
    result[0 * n + 0] = 19.0;
    result[0 * n + 1] = 22.0;
    result[1 * n + 0] = 43.0;
    result[1 * n + 1] = 50.0;

    assert(equalMatrix(result, C, n));

    freeMatrix(A);
    freeMatrix(B);
    freeMatrix(C);
    freeMatrix(result);
}

// -----------------------------
// Test 2: Identidad
// -----------------------------
void test_identity() {
    int n = 4;

    double* A = allocMatrix(n);
    double* I = allocMatrix(n);
    double* C = allocMatrix(n);

    srand(1);
    randomMatrix(A, n);

    zeroMatrix(I, n);
    for (int i = 0; i < n; i++)
        I[i * n + i] = 1.0;

    mult_classic(A, I, C, n);

    assert(equalMatrix(A, C, n));

    freeMatrix(A);
    freeMatrix(I);
    freeMatrix(C);
}

// -----------------------------
// Test 3: Matriz cero
// -----------------------------
void test_zero_matrix_mult() {
    int n = 4;

    double* A = allocMatrix(n);
    double* Z = allocMatrix(n);
    double* C = allocMatrix(n);

    srand(2);
    randomMatrix(A, n);

    zeroMatrix(Z, n);

    mult_classic(A, Z, C, n);

    assert(equalMatrix(Z, C, n));

    freeMatrix(A);
    freeMatrix(Z);
    freeMatrix(C);
}

// -----------------------------
// Test 4: Matriz 5x5
// -----------------------------
void test_multiplicacion_5x5() {
    int n = 5;

    double* A = allocMatrix(n);
    double* B = allocMatrix(n);
    double* C = allocMatrix(n);

    A[0 * n + 0] = 1.0; A[0 * n + 1] = 2.0; A[0 * n + 2] = 3.0; A[0 * n + 3] = 4.0; A[0 * n + 4] = 5.0;
    A[1 * n + 0] = 6.0; A[1 * n + 1] = 7.0; A[1 * n + 2] = 8.0; A[1 * n + 3] = 9.0; A[1 * n + 4] = 10.0;
    A[2 * n + 0] = 11.0; A[2 * n + 1] = 12.0; A[2 * n + 2] = 13.0; A[2 * n + 3] = 14.0; A[2 * n + 4] = 15.0;
    A[3 * n + 0] = 16.0; A[3 * n + 1] = 17.0; A[3 * n + 2] = 18.0; A[3 * n + 3] = 19.0; A[3 * n + 4] = 20.0;
    A[4 * n + 0] = 21.0; A[4 * n + 1] = 22.0; A[4 * n + 2] = 23.0; A[4 * n + 3] = 24.0; A[4 * n + 4] = 25.0;

    B[0 * n + 0] = 25.0; B[0 * n + 1] = 24.0; B[0 * n + 2] = 23.0; B[0 * n + 3] = 22.0; B[0 * n + 4] = 21.0;
    B[1 * n + 0] = 20.0; B[1 * n + 1] = 19.0; B[1 * n + 2] = 18.0; B[1 * n + 3] = 17.0; B[1 * n + 4] = 16.0;
    B[2 * n + 0] = 15.0; B[2 * n + 1] = 14.0; B[2 * n + 2] = 13.0; B[2 * n + 3] = 12.0; B[2 * n + 4] = 11.0;
    B[3 * n + 0] = 10.0; B[3 * n + 1] = 9.0; B[3 * n + 2] = 8.0; B[3 * n + 3] = 7.0; B[3 * n + 4] = 6.0;
    B[4 * n + 0] = 5.0; B[4 * n + 1] = 4.0; B[4 * n + 2] = 3.0; B[4 * n + 3] = 2.0; B[4 * n + 4] = 1.0;

    mult_classic(A, B, C, n);

    double* expected = allocMatrix(n);
    expected[0 * n + 0] = 175.0; expected[0 * n + 1] = 160.0; expected[0 * n + 2] = 145.0; expected[0 * n + 3] = 130.0; expected[0 * n + 4] = 115.0;
    expected[1 * n + 0] = 550.0; expected[1 * n + 1] = 510.0; expected[1 * n + 2] = 470.0; expected[1 * n + 3] = 430.0; expected[1 * n + 4] = 390.0;
    expected[2 * n + 0] = 925.0; expected[2 * n + 1] = 860.0; expected[2 * n + 2] = 795.0; expected[2 * n + 3] = 730.0; expected[2 * n + 4] = 665.0;
    expected[3 * n + 0] = 1300.0; expected[3 * n + 1] = 1210.0; expected[3 * n + 2] = 1120.0; expected[3 * n + 3] = 1030.0; expected[3 * n + 4] = 940.0;
    expected[4 * n + 0] = 1675.0; expected[4 * n + 1] = 1560.0; expected[4 * n + 2] = 1445.0; expected[4 * n + 3] = 1330.0; expected[4 * n + 4] = 1215.0;

    assert(equalMatrix(expected, C, n));

    freeMatrix(A);
    freeMatrix(B);
    freeMatrix(C);
    freeMatrix(expected);
}

// -----------------------------
// MAIN
// -----------------------------
int test_main_mult_matrix() {
    test_multiplicacion_2x2();
    test_identity();
    test_zero_matrix_mult();
    test_multiplicacion_5x5();

    printf("Todos los Tests de mult_classic exitosos =)\n");
    return 0;
}