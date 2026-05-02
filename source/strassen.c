#include <stdlib.h>
#include <string.h>
#include "../interfaces/matrix.h"
#include "../interfaces/strassen.h"

void strassen(double** A, double** B, double** C, int n) {
    if (n == 1) {
        C[0][0] = A[0][0] * B[0][0];
        return;
    }

    int h = n / 2; 

    /* Dividir matrices en 4 submatrices
    *   A = | A11  A12 |    B = | B11  B12 |
    *       | A21  A22 |        | B21  B22 |
    */

    /* Reservar y copiar submatrices A11, A12, A21, A22 */
    double** A11 = allocMatrix(h);
    double** A12 = allocMatrix(h);
    double** A21 = allocMatrix(h);
    double** A22 = allocMatrix(h);

    /* Reservar y copiar submatrices B11, B12, B21, B22 */
    double** B11 = allocMatrix(h);
    double** B12 = allocMatrix(h);
    double** B21 = allocMatrix(h);
    double** B22 = allocMatrix(h);

    for (int i = 0; i < h; i++) {
        memcpy(A11[i], A[i],         h * sizeof(double));
        memcpy(A12[i], A[i] + h,     h * sizeof(double));
        memcpy(A21[i], A[i + h],     h * sizeof(double));
        memcpy(A22[i], A[i + h] + h, h * sizeof(double));

        memcpy(B11[i], B[i],         h * sizeof(double));
        memcpy(B12[i], B[i] + h,     h * sizeof(double));
        memcpy(B21[i], B[i + h],     h * sizeof(double));
        memcpy(B22[i], B[i + h] + h, h * sizeof(double));
    }

    /* 7 matrices para los productos recursivos M1..M7 */
    double** M1 = allocMatrix(h);
    double** M2 = allocMatrix(h);
    double** M3 = allocMatrix(h);
    double** M4 = allocMatrix(h);
    double** M5 = allocMatrix(h);
    double** M6 = allocMatrix(h);
    double** M7 = allocMatrix(h);

    /* T1, T2: matrices auxiliares para sumas/restas antes de recursión */
    double** T1 = allocMatrix(h);
    double** T2 = allocMatrix(h);

    /* M1 = (A11 + A22)(B11 + B22) */
    // T1 = (A11 + A22)
    // T2 = (B11 + B22)
    addMatrix(A11, A22, T1, h);
    addMatrix(B11, B22, T2, h);
    strassen(T1, T2, M1, h);

    /* M2 = (A21 + A22)B11 */
    addMatrix(A21, A22, T1, h);
    strassen(T1, B11, M2, h);

    /* M3 = A11(B12 − B22) */
    subMatrix(B12, B22, T1, h);
    strassen(A11, T1, M3, h);

    /* M4 = A22(B21 − B11) */
    subMatrix(B21, B11, T1, h);
    strassen(A22, T1, M4, h);

    /* M5 = (A11 + A12)B22 */
    addMatrix(A11, A12, T1, h);
    strassen(T1, B22, M5, h);

    /* M6 = (A21 − A11)(B11 + B12) */
    subMatrix(A21, A11, T1, h);
    addMatrix(B11, B12, T2, h);
    strassen(T1, T2, M6, h);

    /* M7 = (A12 − A22)(B21 + B22) */
    subMatrix(A12, A22, T1, h);
    addMatrix(B21, B22, T2, h);
    strassen(T1, T2, M7, h);

    /* Cuadrantes del resultado */
    double** C11 = allocMatrix(h);
    double** C12 = allocMatrix(h);
    double** C21 = allocMatrix(h);
    double** C22 = allocMatrix(h);

    /* C11 = M1 + M4 − M5 + M7 */
    addMatrix(M1, M4, C11, h);
    subMatrix(C11, M5, C11, h);
    addMatrix(C11, M7, C11, h);

    /* C12 = M3 + M5 */
    addMatrix(M3, M5, C12, h);

    /* C21 = M2 + M4 */
    addMatrix(M2, M4, C21, h);

    /* C22 = M1 − M2 + M3 + M6 */
    subMatrix(M1, M2, C22, h);
    addMatrix(C22, M3, C22, h);
    addMatrix(C22, M6, C22, h);

    /* Copiar C11, C12, C21, C22 de vuelta a C
    *   C = | C11  C12 |    
    *       | C21  C22 |   
    */

    for (int i = 0; i < h; i++) {
        memcpy(C[i],     C11[i], h * sizeof(double));
        memcpy(C[i] + h, C12[i], h * sizeof(double));
        memcpy(C[i + h], C21[i], h * sizeof(double));
        memcpy(C[i + h] + h, C22[i], h * sizeof(double));
    }

    /* Liberar toda la memoria auxiliar */
    freeMatrix(A11, h);
    freeMatrix(A12, h);
    freeMatrix(A21, h);
    freeMatrix(A22, h);
    freeMatrix(B11, h);
    freeMatrix(B12, h);
    freeMatrix(B21, h);
    freeMatrix(B22, h);
    freeMatrix(M1, h);
    freeMatrix(M2, h);
    freeMatrix(M3, h);
    freeMatrix(M4, h);
    freeMatrix(M5, h);
    freeMatrix(M6, h);
    freeMatrix(M7, h);
    freeMatrix(T1, h);
    freeMatrix(T2, h);
    freeMatrix(C11, h);
    freeMatrix(C12, h);
    freeMatrix(C21, h);
    freeMatrix(C22, h);
}
