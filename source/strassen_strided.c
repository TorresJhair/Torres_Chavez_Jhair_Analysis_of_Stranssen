#include <stdlib.h>
#include <string.h>
#include "../interfaces/matrix.h"
#include "../interfaces/strassen_strided.h"

static int next_power_of_2(int n) {
    int p = 1;
    while (p < n) p <<= 1;
    return p;
}

static void strassen_recursive_strided(double* A, int a_stride, double* B, int b_stride, double* C, int c_stride, int n) {
    if (n == 1) {
        C[0] = A[0] * B[0];
        return;
    }

    int h = n / 2;

    double* M1 = allocMatrix(h);
    double* M2 = allocMatrix(h);
    double* M3 = allocMatrix(h);
    double* M4 = allocMatrix(h);
    double* M5 = allocMatrix(h);
    double* M6 = allocMatrix(h);
    double* M7 = allocMatrix(h);

    double* T1 = allocMatrix(h);
    double* T2 = allocMatrix(h);

    // M1 = (A11 + A22)(B11 + B22)
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < h; j++) {
            T1[i*h + j] = A[i*a_stride + j] + A[(i+h)*a_stride + (j+h)];
            T2[i*h + j] = B[i*b_stride + j] + B[(i+h)*b_stride + (j+h)];
        }
    }
    strassen_recursive_strided(T1, h, T2, h, M1, h, h);

    // M2 = (A21 + A22)B11
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < h; j++) {
            T1[i*h + j] = A[(i+h)*a_stride + j] + A[(i+h)*a_stride + (j+h)];
        }
    }
    strassen_recursive_strided(T1, h, B, b_stride, M2, h, h);

    // M3 = A11(B12 - B22)
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < h; j++) {
            T1[i*h + j] = B[i*b_stride + (j+h)] - B[(i+h)*b_stride + (j+h)];
        }
    }
    strassen_recursive_strided(A, a_stride, T1, h, M3, h, h);

    // M4 = A22(B21 - B11)
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < h; j++) {
            T1[i*h + j] = B[(i+h)*b_stride + j] - B[i*b_stride + j];
        }
    }
    strassen_recursive_strided(A + h*a_stride + h, a_stride, T1, h, M4, h, h);

    // M5 = (A11 + A12)B22
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < h; j++) {
            T1[i*h + j] = A[i*a_stride + j] + A[i*a_stride + (j+h)];
        }
    }
    strassen_recursive_strided(T1, h, B + h*b_stride + h, b_stride, M5, h, h);

    // M6 = (A21 - A11)(B11 + B12)
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < h; j++) {
            T1[i*h + j] = A[(i+h)*a_stride + j] - A[i*a_stride + j];
            T2[i*h + j] = B[i*b_stride + j] + B[i*b_stride + (j+h)];
        }
    }
    strassen_recursive_strided(T1, h, T2, h, M6, h, h);

    // M7 = (A12 - A22)(B21 + B22)
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < h; j++) {
            T1[i*h + j] = A[i*a_stride + (j+h)] - A[(i+h)*a_stride + (j+h)];
            T2[i*h + j] = B[(i+h)*b_stride + j] + B[(i+h)*b_stride + (j+h)];
        }
    }
    strassen_recursive_strided(T1, h, T2, h, M7, h, h);

    // Combine results directly into C (strided)
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < h; j++) {
            double c11 = M1[i*h + j] + M4[i*h + j] - M5[i*h + j] + M7[i*h + j];
            double c12 = M3[i*h + j] + M5[i*h + j];
            double c21 = M2[i*h + j] + M4[i*h + j];
            double c22 = M1[i*h + j] - M2[i*h + j] + M3[i*h + j] + M6[i*h + j];

            C[i*c_stride + j] = c11;
            C[i*c_stride + (j+h)] = c12;
            C[(i+h)*c_stride + j] = c21;
            C[(i+h)*c_stride + (j+h)] = c22;
        }
    }

    freeMatrix(M1);
    freeMatrix(M2);
    freeMatrix(M3);
    freeMatrix(M4);
    freeMatrix(M5);
    freeMatrix(M6);
    freeMatrix(M7);
    freeMatrix(T1);
    freeMatrix(T2);
}

void strassen_strided(double* A, double* B, double* C, int n) {
    int np2 = next_power_of_2(n);

    if (np2 == n) {
        strassen_recursive_strided(A, n, B, n, C, n, n);
        return;
    }

    double* A_ampliada = allocMatrix(np2);
    double* B_ampliada = allocMatrix(np2);
    double* C_ampliada = allocMatrix(np2);

    zeroMatrix(A_ampliada, np2);
    zeroMatrix(B_ampliada, np2);

    for (int i = 0; i < n; i++) {
        memcpy(A_ampliada + i*np2, A + i*n, n * sizeof(double));
        memcpy(B_ampliada + i*np2, B + i*n, n * sizeof(double));
    }

    strassen_recursive_strided(A_ampliada, np2, B_ampliada, np2, C_ampliada, np2, np2);

    for (int i = 0; i < n; i++) {
        memcpy(C + i*n, C_ampliada + i*np2, n * sizeof(double));
    }

    freeMatrix(A_ampliada);
    freeMatrix(B_ampliada);
    freeMatrix(C_ampliada);
}
