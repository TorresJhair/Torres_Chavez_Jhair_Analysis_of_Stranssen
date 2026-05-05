#ifndef BENCHMARK_H
#define BENCHMARK_H

#include <time.h>

// Retorna la diferencia entre dos tiempos en nanosegundos
long long elapsedNanoseconds(struct timespec start, struct timespec end);

// Mide el tiempo de mult_classic en microsegundos (1/1000 ms)
double measureClassicMult(double* A, double* B, double* C, int n);

// Mide el tiempo de strassen en microsegundos (1/1000 ms)
double measureStrassen(double* A, double* B, double* C, int n);

// Mide el tiempo de strassen_strided en microsegundos (1/1000 ms)
double measureStrassenStrided(double* A, double* B, double* C, int n);

#endif
