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

#endif