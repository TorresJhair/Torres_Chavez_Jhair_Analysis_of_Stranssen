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
    printf("Test alloc/free: OK\n");
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

int main() {
    printf("Tests Matrix\n");

    test_alloc_free();
    test_copy_matrix();
    test_random_matrix_range();
    test_zero_matrix();

    printf("Todos los Test de Matrix exitosos =)\n");
    return 0;
}