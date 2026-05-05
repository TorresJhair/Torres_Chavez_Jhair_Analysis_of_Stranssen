// Necesario para usar clock_gettime y CLOCK_MONOTONIC en C
#define _POSIX_C_SOURCE 199309L

#include <stdlib.h>
#include <time.h>
#include "../interfaces/benchmark.h"
#include "../interfaces/mult_classic.h"
#include "../interfaces/strassen.h"

// struct timespec almacena el tiempo en segundos (tv_sec) y nanosegundos (tv_nsec)
// con alta precisión. 

// Convierte la diferencia de dos struct timespec a nanosegundos
// 1 nanosegundo = 0.000000001 segundos 10^(-9) segs
long long elapsedNanoseconds(struct timespec start, struct timespec end) {
    long long seconds = (long long)(end.tv_sec - start.tv_sec);
    long long nanoseconds = (long long)(end.tv_nsec - start.tv_nsec);
    // Devuelve total en nanosegundos
    return seconds * 1000000000LL + nanoseconds;
}

// CLOCK_MONOTONIC se usa porque no se ve afectado por cambios de hora
// del sistema (ajuste de red, ajustes manuales), como CLOCK_REALTIME,
// garantizando mediciones precisas.
double measureClassicMult(double* A, double* B, double* C, int n) {
    struct timespec start;
    struct timespec end;

    // Captura el tiempo antes y después con clock_gettime
    // Toma la hora de CLOCK_MONOTONIC y lo mete en &start
    clock_gettime(CLOCK_MONOTONIC, &start);
    mult_classic(A, B, C, n);
    clock_gettime(CLOCK_MONOTONIC, &end);

    // Devuelve el resultado en microsegundos
    // 1 Microsegundo : (0.000001 s) 10^(-6) segs 
    return (double)elapsedNanoseconds(start, end) / 1000.0;
}

// Mismo esquema de medición que measureClassicMult para que sean comparables
double measureStrassen(double* A, double* B, double* C, int n) {
    struct timespec start;
    struct timespec end;

    clock_gettime(CLOCK_MONOTONIC, &start);
    strassen(A, B, C, n);
    clock_gettime(CLOCK_MONOTONIC, &end);

    return (double)elapsedNanoseconds(start, end) / 1000.0;
}
