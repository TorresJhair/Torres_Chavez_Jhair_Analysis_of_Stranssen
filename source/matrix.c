#include <stdlib.h>
#include <string.h>
#include "../interfaces/matrix.h"
#include <math.h>

double** allocMatrix(int n) {
    double** M = malloc(n * sizeof(double*));
    for (int i = 0; i < n; i++)
        M[i] = malloc(n * sizeof(double));
    return M;
}

void freeMatrix(double** M, int n) {
    for (int i = 0; i < n; i++)
        free(M[i]);
    free(M);
}

void copyMatrix(double** A, double** B, int n) {
    for (int i = 0; i < n; i++)
        memcpy(B[i], A[i], n * sizeof(double));
}

void randomMatrix(double** A, int n) {
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            A[i][j] = rand() % 10;
}

void zeroMatrix(double** M, int n) {
    for (int i = 0; i < n; i++)
        memset(M[i], 0, n * sizeof(double));
}

int equalMatrix(double** A, double** B, int n) {
    // Tolerancia absoluta para comparar doubles
    double EPS =  1e-9;

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {

            double a = A[i][j];
            double b = B[i][j];
            double diff = fabs(a - b);

            // comparación robusta (relativa + absoluta)
            double scale = fmax(fabs(a), fabs(b));

            // Tolerancia absoluta: EPS, falla para números grandes: 
            // a = 1,000,000
            // b = 1,000,000.000001
            // diferencia = 1e-6, EPS = 1e-9

            // Tolerancia relativa (1.0 + scale): ayuda con los números grandes.
            // Importa qué tan grande es el error comparado con el valor
            
            // Analogía simple
            // Error de 1 dólar: en 10 dólares → mucho (10%)
            // en 1,000,000 dólares → insignificante
            if (diff > EPS * (1.0 + scale)) {
                return 0;
            }
        }
    }
    return 1;
}