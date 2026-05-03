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

    double** A = allocMatrix(n);
    double** B = allocMatrix(n);
    double** C = allocMatrix(n);

    // A = [1 2; 3 4]
    A[0][0] = 1.0; A[0][1] = 2.0;
    A[1][0] = 3.0; A[1][1] = 4.0;

    // B = [5 6; 7 8]
    B[0][0] = 5.0; B[0][1] = 6.0;
    B[1][0] = 7.0; B[1][1] = 8.0;

    mult_classic(A, B, C, n);

    double** result = allocMatrix(n);
    result[0][0] = 19.0;
    result[0][1] = 22.0;
    result[1][0] = 43.0;
    result[1][1] = 50.0;

    assert(equalMatrix(result, C, n));

    freeMatrix(A,n);
    freeMatrix(B,n);
    freeMatrix(C,n);
    freeMatrix(result,n);

}

// -----------------------------
// Test 2: Identidad
// -----------------------------
void test_identity() {
    int n = 4;

    double** A = allocMatrix(n);
    double** I = allocMatrix(n);
    double** C = allocMatrix(n);

    srand(1);
    randomMatrix(A, n);

    zeroMatrix(I, n);
    for (int i = 0; i < n; i++)
        I[i][i] = 1.0;

    mult_classic(A, I, C, n);

    assert(equalMatrix(A, C, n));

    freeMatrix(A,n);
    freeMatrix(I,n);
    freeMatrix(C,n);
}

// -----------------------------
// Test 3: Matriz cero
// -----------------------------
void test_zero_matrix_mult() {
    int n = 4;

    double** A = allocMatrix(n);
    double** Z = allocMatrix(n);
    double** C = allocMatrix(n);

    srand(2);
    randomMatrix(A, n);

    zeroMatrix(Z, n);

    mult_classic(A, Z, C, n);

    assert(equalMatrix(Z, C, n));

    freeMatrix(A,n);
    freeMatrix(Z,n);
    freeMatrix(C,n);
}

// -----------------------------
// Test 4: Matriz 5x5
// -----------------------------
void test_multiplicacion_5x5() {
    int n = 5;

    double** A = allocMatrix(n);
    double** B = allocMatrix(n);
    double** C = allocMatrix(n);

    A[0][0] = 1.0; A[0][1] = 2.0; A[0][2] = 3.0; A[0][3] = 4.0; A[0][4] = 5.0;
    A[1][0] = 6.0; A[1][1] = 7.0; A[1][2] = 8.0; A[1][3] = 9.0; A[1][4] = 10.0;
    A[2][0] = 11.0; A[2][1] = 12.0; A[2][2] = 13.0; A[2][3] = 14.0; A[2][4] = 15.0;
    A[3][0] = 16.0; A[3][1] = 17.0; A[3][2] = 18.0; A[3][3] = 19.0; A[3][4] = 20.0;
    A[4][0] = 21.0; A[4][1] = 22.0; A[4][2] = 23.0; A[4][3] = 24.0; A[4][4] = 25.0;

    B[0][0] = 25.0; B[0][1] = 24.0; B[0][2] = 23.0; B[0][3] = 22.0; B[0][4] = 21.0;
    B[1][0] = 20.0; B[1][1] = 19.0; B[1][2] = 18.0; B[1][3] = 17.0; B[1][4] = 16.0;
    B[2][0] = 15.0; B[2][1] = 14.0; B[2][2] = 13.0; B[2][3] = 12.0; B[2][4] = 11.0;
    B[3][0] = 10.0; B[3][1] = 9.0; B[3][2] = 8.0; B[3][3] = 7.0; B[3][4] = 6.0;
    B[4][0] = 5.0; B[4][1] = 4.0; B[4][2] = 3.0; B[4][3] = 2.0; B[4][4] = 1.0;

    mult_classic(A, B, C, n);

    double** expected = allocMatrix(n);
    expected[0][0] = 175.0; expected[0][1] = 160.0; expected[0][2] = 145.0; expected[0][3] = 130.0; expected[0][4] = 115.0;
    expected[1][0] = 550.0; expected[1][1] = 510.0; expected[1][2] = 470.0; expected[1][3] = 430.0; expected[1][4] = 390.0;
    expected[2][0] = 925.0; expected[2][1] = 860.0; expected[2][2] = 795.0; expected[2][3] = 730.0; expected[2][4] = 665.0;
    expected[3][0] = 1300.0; expected[3][1] = 1210.0; expected[3][2] = 1120.0; expected[3][3] = 1030.0; expected[3][4] = 940.0;
    expected[4][0] = 1675.0; expected[4][1] = 1560.0; expected[4][2] = 1445.0; expected[4][3] = 1330.0; expected[4][4] = 1215.0;

    assert(equalMatrix(expected, C, n));

    freeMatrix(A, n);
    freeMatrix(B, n);
    freeMatrix(C, n);
    freeMatrix(expected, n);
}

// -----------------------------
// MAIN
// -----------------------------
int test_main_mult_matrix() {
    test_multiplicacion_2x2();
    test_identity();
    test_zero_matrix_mult();
    test_multiplicacion_5x5();

    printf("Todos los Tests de mult_classic existosos =)\n");
    return 0;
}