#ifndef MATRIX_H
#define MATRIX_H

// Reservar memoria, solo matrices n x n
double** allocMatrix(int n);

// Liberar memoria matriz M de orden n x n
void freeMatrix(double** M, int n);

// Copiar Matriz B (origen) en Matriz A (destino)
void copyMatrix(double** A, double** B, int n);

// Rellenar valores random a matriz A
void randomMatrix(double** A, int n);

// Rellenar con 0's la matriz M
void zeroMatrix(double** M, int n);

// Compara dos matrices, retorna 1 si son iguales; 0 si no son iguales
int equalMatrix(double** A, double** B, int n);

// Suma dos matrices (C = A + B)
void addMatrix(double** A, double** B, double** C, int n);

// Resta dos matrices (C = A - B)
void subMatrix(double** A, double** B, double** C, int n);

#endif