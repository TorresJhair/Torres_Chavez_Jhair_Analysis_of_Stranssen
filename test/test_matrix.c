#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include "../interfaces/matrix.h"

// Debería reservar y liberar memoria correctamente
// Verificar manejo memoria, no importa los valores dentro de la matriz
void test_alloc_free() {
    int n = 4;
    double **A = allocMatrix(n);

    assert(A != NULL);

    for (int i = 0; i < n; i++)
        assert(A[i] != NULL);

    freeMatrix(A, n);
}

// Debería copiar los valores de una Matriz A (origen) a Matriz B (destino)
void test_copy_matrix() {
    int n = 3;

    double** A = allocMatrix(n);
    double** B = allocMatrix(n);

    // llenar A con valores conocidos
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            A[i][j] = i * n + j + 0.5;

    copyMatrix(A, B, n);

    // verificar copia exacta
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            assert(A[i][j] == B[i][j]);

    freeMatrix(A, n);
    freeMatrix(B, n);
}

// Debería rellenar con valores aleatorios, generados entre 0 <= x < 10
// Semilla fijada en 42 
void test_random_matrix_range() {
    int n = 5;

    double** A = allocMatrix(n);

    srand(42);  // fijar semilla

    randomMatrix(A, n);

    // verificar rango [0, 9]
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++) {
            assert(A[i][j] >= 0);
            assert(A[i][j] < 10);
        }

    freeMatrix(A, n);
}

// Debería colocar todos los valores de la Matriz en 0
// No importa si esta ya antes tenía valores asignados
void test_zero_matrix() {
    int n = 4;

    double** M = allocMatrix(n);

    // llenar con basura
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            M[i][j] = i + j + 5;

    zeroMatrix(M, n);

    // verificar todo cero
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            assert(M[i][j] == 0);

    freeMatrix(M, n);
}

void test_add_matrix() {
    int n = 3;

    double** A = allocMatrix(n);
    double** B = allocMatrix(n);
    double** C = allocMatrix(n);
    double** expected = allocMatrix(n);

    A[0][0] = 1.0; A[0][1] = 2.0; A[0][2] = 3.0;
    A[1][0] = 4.0; A[1][1] = 5.0; A[1][2] = 6.0;
    A[2][0] = 7.0; A[2][1] = 8.0; A[2][2] = 9.0;

    B[0][0] = 9.0; B[0][1] = 8.0; B[0][2] = 7.0;
    B[1][0] = 6.0; B[1][1] = 5.0; B[1][2] = 4.0;
    B[2][0] = 3.0; B[2][1] = 2.0; B[2][2] = 1.0;

    expected[0][0] = 10.0; expected[0][1] = 10.0; expected[0][2] = 10.0;
    expected[1][0] = 10.0; expected[1][1] = 10.0; expected[1][2] = 10.0;
    expected[2][0] = 10.0; expected[2][1] = 10.0; expected[2][2] = 10.0;

    addMatrix(A, B, C, n);

    assert(equalMatrix(expected, C, n));

    freeMatrix(A, n);
    freeMatrix(B, n);
    freeMatrix(C, n);
    freeMatrix(expected, n);
}

void test_sub_matrix() {
    int n = 3;

    double** A = allocMatrix(n);
    double** B = allocMatrix(n);
    double** C = allocMatrix(n);
    double** expected = allocMatrix(n);

    A[0][0] = 5.0; A[0][1] = 7.0; A[0][2] = 9.0;
    A[1][0] = 3.0; A[1][1] = 2.0; A[1][2] = 6.0;
    A[2][0] = 8.0; A[2][1] = 1.0; A[2][2] = 4.0;

    B[0][0] = 2.0; B[0][1] = 3.0; B[0][2] = 1.0;
    B[1][0] = 1.0; B[1][1] = 1.0; B[1][2] = 2.0;
    B[2][0] = 4.0; B[2][1] = 0.0; B[2][2] = 3.0;

    expected[0][0] = 3.0; expected[0][1] = 4.0; expected[0][2] = 8.0;
    expected[1][0] = 2.0; expected[1][1] = 1.0; expected[1][2] = 4.0;
    expected[2][0] = 4.0; expected[2][1] = 1.0; expected[2][2] = 1.0;

    subMatrix(A, B, C, n);

    assert(equalMatrix(expected, C, n));

    freeMatrix(A, n);
    freeMatrix(B, n);
    freeMatrix(C, n);
    freeMatrix(expected, n);
}

int test_main_matrix() {
    printf("Tests Matrix\n");

    test_alloc_free();
    test_copy_matrix();
    test_random_matrix_range();
    test_zero_matrix();
    test_add_matrix();
    test_sub_matrix();

    printf("Todos los Test de Matrix exitosos =)\n");
    return 0;
}