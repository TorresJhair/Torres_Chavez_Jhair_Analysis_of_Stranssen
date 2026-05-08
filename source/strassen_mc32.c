#include <stdlib.h>
#include <string.h>
#include "../interfaces/matrix.h"
#include "../interfaces/strassen_mc32.h"
#include "../interfaces/mult_classic.h"

#define CUTOFF 32

static int next_multiple(int n, int multiple) {
    return ((n + multiple - 1) / multiple) * multiple;
}

static int workspace_size(int n) {
    if (n <= CUTOFF) return 0;
    int h = n / 2;
    return 9 * h * h + workspace_size(h);
}

static void strassen_recursive_strided_ws(double* restrict A, int a_stride,
                                           double* restrict B, int b_stride,
                                           double* restrict C, int c_stride,
                                           int n, double* ws) {

    if (n <= CUTOFF) {
        if (a_stride == n && b_stride == n && c_stride == n) {
            mult_classic(A, B, C, n);
        } else {
            double* A_contig = allocMatrix(n);
            double* B_contig = allocMatrix(n);
            double* C_contig = allocMatrix(n);

            for (int i = 0; i < n; i++)
                for (int j = 0; j < n; j++) {
                    A_contig[i*n + j] = A[i*a_stride + j];
                    B_contig[i*n + j] = B[i*b_stride + j];
                }

            mult_classic(A_contig, B_contig, C_contig, n);

            for (int i = 0; i < n; i++)
                for (int j = 0; j < n; j++)
                    C[i*c_stride + j] = C_contig[i*n + j];

            freeMatrix(A_contig);
            freeMatrix(B_contig);
            freeMatrix(C_contig);
        }
        return;
    }

    int h    = n / 2;
    int h_sq = h * h;

    double* M1       = ws + 0 * h_sq;
    double* M2       = ws + 1 * h_sq;
    double* M3       = ws + 2 * h_sq;
    double* M4       = ws + 3 * h_sq;
    double* M5       = ws + 4 * h_sq;
    double* M6       = ws + 5 * h_sq;
    double* M7       = ws + 6 * h_sq;
    double* T1       = ws + 7 * h_sq;
    double* T2       = ws + 8 * h_sq;
    double* child_ws = ws + 9 * h_sq;

    for (int i = 0; i < h; i++)
        for (int j = 0; j < h; j++) {
            T1[i*h + j] = A[i*a_stride + j]         + A[(i+h)*a_stride + (j+h)];
            T2[i*h + j] = B[i*b_stride + j]         + B[(i+h)*b_stride + (j+h)];
        }
    strassen_recursive_strided_ws(T1, h, T2, h, M1, h, h, child_ws);

    for (int i = 0; i < h; i++)
        for (int j = 0; j < h; j++)
            T1[i*h + j] = A[(i+h)*a_stride + j] + A[(i+h)*a_stride + (j+h)];
    strassen_recursive_strided_ws(T1, h, B, b_stride, M2, h, h, child_ws);

    for (int i = 0; i < h; i++)
        for (int j = 0; j < h; j++)
            T1[i*h + j] = B[i*b_stride + (j+h)] - B[(i+h)*b_stride + (j+h)];
    strassen_recursive_strided_ws(A, a_stride, T1, h, M3, h, h, child_ws);

    for (int i = 0; i < h; i++)
        for (int j = 0; j < h; j++)
            T1[i*h + j] = B[(i+h)*b_stride + j] - B[i*b_stride + j];
    strassen_recursive_strided_ws(A + h*a_stride + h, a_stride, T1, h, M4, h, h, child_ws);

    for (int i = 0; i < h; i++)
        for (int j = 0; j < h; j++)
            T1[i*h + j] = A[i*a_stride + j] + A[i*a_stride + (j+h)];
    strassen_recursive_strided_ws(T1, h, B + h*b_stride + h, b_stride, M5, h, h, child_ws);

    for (int i = 0; i < h; i++)
        for (int j = 0; j < h; j++) {
            T1[i*h + j] = A[(i+h)*a_stride + j] - A[i*a_stride + j];
            T2[i*h + j] = B[i*b_stride + j]     + B[i*b_stride + (j+h)];
        }
    strassen_recursive_strided_ws(T1, h, T2, h, M6, h, h, child_ws);

    for (int i = 0; i < h; i++)
        for (int j = 0; j < h; j++) {
            T1[i*h + j] = A[i*a_stride + (j+h)]     - A[(i+h)*a_stride + (j+h)];
            T2[i*h + j] = B[(i+h)*b_stride + j]     + B[(i+h)*b_stride + (j+h)];
        }
    strassen_recursive_strided_ws(T1, h, T2, h, M7, h, h, child_ws);

    for (int i = 0; i < h; i++)
        for (int j = 0; j < h; j++) {
            C[ i   *c_stride +  j   ] = M1[i*h+j] + M4[i*h+j] - M5[i*h+j] + M7[i*h+j];
            C[ i   *c_stride + (j+h)] = M3[i*h+j] + M5[i*h+j];
            C[(i+h)*c_stride +  j   ] = M2[i*h+j] + M4[i*h+j];
            C[(i+h)*c_stride + (j+h)] = M1[i*h+j] - M2[i*h+j] + M3[i*h+j] + M6[i*h+j];
        }
}

void strassen_mc32(double* A, double* B, double* C, int n) {
    if (n <= CUTOFF) {
        mult_classic(A, B, C, n);
        return;
    }

    int np = next_multiple(n, CUTOFF);

    int ws_size  = workspace_size(np);
    double* workspace = (ws_size > 0) ? (double*)malloc(ws_size * sizeof(double)) : NULL;

    if (np == n) {
        strassen_recursive_strided_ws(A, n, B, n, C, n, n, workspace);
        if (workspace) free(workspace);
        return;
    }

    double* Ap = (double*)calloc(np * np, sizeof(double));
    double* Bp = (double*)calloc(np * np, sizeof(double));
    double* Cp = (double*)calloc(np * np, sizeof(double));

    for (int i = 0; i < n; i++) {
        memcpy(Ap + i*np, A + i*n, n * sizeof(double));
        memcpy(Bp + i*np, B + i*n, n * sizeof(double));
    }

    strassen_recursive_strided_ws(Ap, np, Bp, np, Cp, np, np, workspace);

    for (int i = 0; i < n; i++)
        memcpy(C + i*n, Cp + i*np, n * sizeof(double));

    free(Ap);
    free(Bp);
    free(Cp);
    if (workspace) free(workspace);
}