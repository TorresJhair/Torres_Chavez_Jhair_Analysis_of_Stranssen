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

int main() {
    printf("Tests Matrix\n");

    test_alloc_free();

    printf("Todos los Test de Matrix exitosos =)\n");
    return 0;
}