#include <stdlib.h>
#include <string.h>
#include "../interfaces/matrix.h"
#include "../interfaces/strassen_strided.h"

#define CUTOFF 32

static int next_multiple_of_cutoff(int n) {
    if (n % CUTOFF == 0) return n;
    return ((n / CUTOFF) + 1) * CUTOFF;
}

// Classic multiplication for strided matrices (used below cutoff)
static void mult_classic_strided(double* restrict A, int a_stride,
                                   double* restrict B, int b_stride,
                                   double* restrict C, int c_stride, int n) {
    // Zero C
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            C[i*c_stride + j] = 0;

    for (int i = 0; i < n; i++)
        for (int k = 0; k < n; k++) {
            double a_ik = A[i*a_stride + k];
            for (int j = 0; j < n; j++)
                C[i*c_stride + j] += a_ik * B[k*b_stride + j];
        }
}

// Compute workspace size: 9*(n/2)^2 at each level until cutoff
static int workspace_size(int n) {
    if (n <= CUTOFF) return 0;
    int h = n / 2;
    if (h <= CUTOFF)
        return 9 * h * h;
    return 9 * h * h + workspace_size(h);
}

static void strassen_recursive_strided_ws(double* restrict A, int a_stride,
                                            double* restrict B, int b_stride,
                                            double* restrict C, int c_stride,
                                            int n, double* ws) {

    // Cutoff: use classic for small matrices
    if (n <= CUTOFF) {
        mult_classic_strided(A, a_stride, B, b_stride, C, c_stride, n);
        return;
    }

    int h = n / 2;
    int h_sq = h * h;

    double* M1 = ws;
    double* M2 = ws + 1 * h_sq;
    double* M3 = ws + 2 * h_sq;
    double* M4 = ws + 3 * h_sq;
    double* M5 = ws + 4 * h_sq;
    double* M6 = ws + 5 * h_sq;
    double* M7 = ws + 6 * h_sq;
    double* T1 = ws + 7 * h_sq;
    double* T2 = ws + 8 * h_sq;
    double* child_ws = ws + 9 * h_sq;

    // M1 = (A11 + A22)(B11 + B22)
    for (int i = 0; i < h; i++)
        for (int j = 0; j < h; j++) {
            T1[i*h + j] = A[i*a_stride + j] + A[(i+h)*a_stride + (j+h)];
            T2[i*h + j] = B[i*b_stride + j] + B[(i+h)*b_stride + (j+h)];
        }
    strassen_recursive_strided_ws(T1, h, T2, h, M1, h, h, child_ws);

    // M2 = (A21 + A22)B11
    for (int i = 0; i < h; i++)
        for (int j = 0; j < h; j++)
            T1[i*h + j] = A[(i+h)*a_stride + j] + A[(i+h)*a_stride + (j+h)];
    strassen_recursive_strided_ws(T1, h, B, b_stride, M2, h, h, child_ws);

    // M3 = A11(B12 - B22)
    for (int i = 0; i < h; i++)
        for (int j = 0; j < h; j++)
            T1[i*h + j] = B[i*b_stride + (j+h)] - B[(i+h)*b_stride + (j+h)];
    strassen_recursive_strided_ws(A, a_stride, T1, h, M3, h, h, child_ws);

    // M4 = A22(B21 - B11)
    for (int i = 0; i < h; i++)
        for (int j = 0; j < h; j++)
            T1[i*h + j] = B[(i+h)*b_stride + j] - B[i*b_stride + j];
    strassen_recursive_strided_ws(A + h*a_stride + h, a_stride, T1, h, M4, h, h, child_ws);

    // M5 = (A11 + A12)B22
    for (int i = 0; i < h; i++)
        for (int j = 0; j < h; j++)
            T1[i*h + j] = A[i*a_stride + j] + A[i*a_stride + (j+h)];
    strassen_recursive_strided_ws(T1, h, B + h*b_stride + h, b_stride, M5, h, h, child_ws);

    // M6 = (A21 - A11)(B11 + B12)
    for (int i = 0; i < h; i++)
        for (int j = 0; j < h; j++) {
            T1[i*h + j] = A[(i+h)*a_stride + j] - A[i*a_stride + j];
            T2[i*h + j] = B[i*b_stride + j] + B[i*b_stride + (j+h)];
        }
    strassen_recursive_strided_ws(T1, h, T2, h, M6, h, h, child_ws);

    // M7 = (A12 - A22)(B21 + B22)
    for (int i = 0; i < h; i++)
        for (int j = 0; j < h; j++) {
            T1[i*h + j] = A[i*a_stride + (j+h)] - A[(i+h)*a_stride + (j+h)];
            T2[i*h + j] = B[(i+h)*b_stride + j] + B[(i+h)*b_stride + (j+h)];
        }
    strassen_recursive_strided_ws(T1, h, T2, h, M7, h, h, child_ws);

    // Combine results into C
    for (int i = 0; i < h; i++)
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

void strassen_nearest_cutoff(double* A, double* B, double* C, int n) {
    int np2 = next_multiple_of_cutoff(n);

    int ws_size = workspace_size(np2);
    double* workspace = NULL;
    if (ws_size > 0)
        workspace = (double*)malloc(ws_size * sizeof(double));

    if (np2 == n) {
        strassen_recursive_strided_ws(A, n, B, n, C, n, n, workspace);
        if (workspace) free(workspace);
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

    strassen_recursive_strided_ws(A_ampliada, np2, B_ampliada, np2, C_ampliada, np2, np2, workspace);

    for (int i = 0; i < n; i++)
        memcpy(C + i*n, C_ampliada + i*np2, n * sizeof(double));

    freeMatrix(A_ampliada);
    freeMatrix(B_ampliada);
    freeMatrix(C_ampliada);
    if (workspace) free(workspace);
}
