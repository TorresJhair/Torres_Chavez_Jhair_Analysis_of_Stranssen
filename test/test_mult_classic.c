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
// MAIN
// -----------------------------
int test_main_mult_matrix() {
    test_multiplicacion_2x2();
    test_identity();
    test_zero_matrix_mult();

    printf("Todos los Tests de mult_classic existosos =)\n");
    return 0;
}