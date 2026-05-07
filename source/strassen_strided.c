#include <stdlib.h>
#include <string.h>
#include "../interfaces/matrix.h"
#include "../interfaces/strassen_strided.h"
#include "../interfaces/mult_classic.h"

#define CUTOFF 32

// Round n up to the nearest multiple of 'multiple'
static int next_multiple(int n, int multiple) {
    return ((n + multiple - 1) / multiple) * multiple;
}

// Workspace needed: 9 * h^2 per level, recursing on h = np/2 until CUTOFF
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

    // M1 = (A11 + A22)(B11 + B22)
    for (int i = 0; i < h; i++)
        for (int j = 0; j < h; j++) {
            T1[i*h + j] = A[i*a_stride + j]         + A[(i+h)*a_stride + (j+h)];
            T2[i*h + j] = B[i*b_stride + j]         + B[(i+h)*b_stride + (j+h)];
        }
    strassen_recursive_strided_ws(T1, h, T2, h, M1, h, h, child_ws);

    // M2 = (A21 + A22) * B11
    for (int i = 0; i < h; i++)
        for (int j = 0; j < h; j++)
            T1[i*h + j] = A[(i+h)*a_stride + j] + A[(i+h)*a_stride + (j+h)];
    strassen_recursive_strided_ws(T1, h, B, b_stride, M2, h, h, child_ws);

    // M3 = A11 * (B12 - B22)
    for (int i = 0; i < h; i++)
        for (int j = 0; j < h; j++)
            T1[i*h + j] = B[i*b_stride + (j+h)] - B[(i+h)*b_stride + (j+h)];
    strassen_recursive_strided_ws(A, a_stride, T1, h, M3, h, h, child_ws);

    // M4 = A22 * (B21 - B11)
    for (int i = 0; i < h; i++)
        for (int j = 0; j < h; j++)
            T1[i*h + j] = B[(i+h)*b_stride + j] - B[i*b_stride + j];
    strassen_recursive_strided_ws(A + h*a_stride + h, a_stride, T1, h, M4, h, h, child_ws);

    // M5 = (A11 + A12) * B22
    for (int i = 0; i < h; i++)
        for (int j = 0; j < h; j++)
            T1[i*h + j] = A[i*a_stride + j] + A[i*a_stride + (j+h)];
    strassen_recursive_strided_ws(T1, h, B + h*b_stride + h, b_stride, M5, h, h, child_ws);

    // M6 = (A21 - A11)(B11 + B12)
    for (int i = 0; i < h; i++)
        for (int j = 0; j < h; j++) {
            T1[i*h + j] = A[(i+h)*a_stride + j] - A[i*a_stride + j];
            T2[i*h + j] = B[i*b_stride + j]     + B[i*b_stride + (j+h)];
        }
    strassen_recursive_strided_ws(T1, h, T2, h, M6, h, h, child_ws);

    // M7 = (A12 - A22)(B21 + B22)
    for (int i = 0; i < h; i++)
        for (int j = 0; j < h; j++) {
            T1[i*h + j] = A[i*a_stride + (j+h)]     - A[(i+h)*a_stride + (j+h)];
            T2[i*h + j] = B[(i+h)*b_stride + j]     + B[(i+h)*b_stride + (j+h)];
        }
    strassen_recursive_strided_ws(T1, h, T2, h, M7, h, h, child_ws);

    // Combine into C
    for (int i = 0; i < h; i++)
        for (int j = 0; j < h; j++) {
            C[ i   *c_stride +  j   ] = M1[i*h+j] + M4[i*h+j] - M5[i*h+j] + M7[i*h+j];
            C[ i   *c_stride + (j+h)] = M3[i*h+j] + M5[i*h+j];
            C[(i+h)*c_stride +  j   ] = M2[i*h+j] + M4[i*h+j];
            C[(i+h)*c_stride + (j+h)] = M1[i*h+j] - M2[i*h+j] + M3[i*h+j] + M6[i*h+j];
        }
}

void strassen_strided(double* A, double* B, double* C, int n) {
    // Pad to nearest multiple of CUTOFF instead of next power of 2.
    // With CUTOFF=32 (a power of 2), every recursive halving stays exact
    // all the way down to the base case, so no further padding is needed.
    //
    // Example: n=257 → np=288  (was 512 with power-of-2 padding)
    //          n=513 → np=544  (was 1024)
    
    if (n <= CUTOFF) {
        // Caso base directo, sin padding alguno
        mult_classic(A, B, C, n);
        return;
    }
    
    int np = next_multiple(n, CUTOFF);

    int ws_size  = workspace_size(np);
    double* workspace = (ws_size > 0) ? (double*)malloc(ws_size * sizeof(double)) : NULL;

    if (np == n) {
        // No padding needed — operate directly on the input matrices
        strassen_recursive_strided_ws(A, n, B, n, C, n, n, workspace);
        if (workspace) free(workspace);
        return;
    }

    // Allocate zero-initialised padded matrices
    double* Ap = (double*)calloc(np * np, sizeof(double));
    double* Bp = (double*)calloc(np * np, sizeof(double));
    double* Cp = (double*)calloc(np * np, sizeof(double));

    // Copy rows of A and B into the padded buffers (extra columns stay 0)
    for (int i = 0; i < n; i++) {
        memcpy(Ap + i*np, A + i*n, n * sizeof(double));
        memcpy(Bp + i*np, B + i*n, n * sizeof(double));
    }

    strassen_recursive_strided_ws(Ap, np, Bp, np, Cp, np, np, workspace);

    // Extract the n×n result
    for (int i = 0; i < n; i++)
        memcpy(C + i*n, Cp + i*np, n * sizeof(double));

    free(Ap);
    free(Bp);
    free(Cp);
    if (workspace) free(workspace);
}