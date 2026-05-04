#include <stdlib.h>
#include <string.h>
#include "../interfaces/matrix.h"
#include "../interfaces/strassen.h"

// Calcula la menor potencia de 2 mayor o igual a n.
// Se usa para determinar el tamaño de padding necesario
// cuando la matriz original no tiene dimensiones válidas para Strassen.
static int next_power_of_2(int n) {
    int p = 1;
    while (p < n) p <<= 1;
    return p;
}

// Asume que n es potencia de 2. Divide las matrices en cuadrantes,
// realiza 7 multiplicaciones recursivas y combina los resultados.
static void strassen_recursive(double* A, double* B, double* C, int n) {
    if (n == 1) {
        C[0] = A[0] * B[0];
        return;
    }

    int h = n / 2;

    // División de cada matriz en 4 cuadrantes de tamaño h×h:
    //    A = | A11  A12 |    B = | B11  B12 |
    //        | A21  A22 |        | B21  B22 |

    // Reservar y copiar los 4 cuadrantes de A
    double* A11 = allocMatrix(h);
    double* A12 = allocMatrix(h);
    double* A21 = allocMatrix(h);
    double* A22 = allocMatrix(h);

    // Reservar y copiar los 4 cuadrantes de B
    double* B11 = allocMatrix(h);
    double* B12 = allocMatrix(h);
    double* B21 = allocMatrix(h);
    double* B22 = allocMatrix(h);

    for (int i = 0; i < h; i++) {
        memcpy(A11 + i*h, A + i*n,         h * sizeof(double));
        memcpy(A12 + i*h, A + i*n + h,     h * sizeof(double));
        memcpy(A21 + i*h, A + (i+h)*n,     h * sizeof(double));
        memcpy(A22 + i*h, A + (i+h)*n + h, h * sizeof(double));

        memcpy(B11 + i*h, B + i*n,         h * sizeof(double));
        memcpy(B12 + i*h, B + i*n + h,     h * sizeof(double));
        memcpy(B21 + i*h, B + (i+h)*n,     h * sizeof(double));
        memcpy(B22 + i*h, B + (i+h)*n + h, h * sizeof(double));
    }

    // 7 productos recursivos M1..M7 según la fórmula de Strassen
    double* M1 = allocMatrix(h);
    double* M2 = allocMatrix(h);
    double* M3 = allocMatrix(h);
    double* M4 = allocMatrix(h);
    double* M5 = allocMatrix(h);
    double* M6 = allocMatrix(h);
    double* M7 = allocMatrix(h);

    // T1 y T2: matrices temporales para las sumas y restas previas a la recursión
    double* T1 = allocMatrix(h);
    double* T2 = allocMatrix(h);

    /* M1 = (A11 + A22)(B11 + B22) */
    // T1 = (A11 + A22)
    // T2 = (B11 + B22)
    addMatrix(A11, A22, T1, h);
    addMatrix(B11, B22, T2, h);
    strassen_recursive(T1, T2, M1, h);

    /* M2 = (A21 + A22)B11 */
    addMatrix(A21, A22, T1, h);
    strassen_recursive(T1, B11, M2, h);

    /* M3 = A11(B12 − B22) */
    subMatrix(B12, B22, T1, h);
    strassen_recursive(A11, T1, M3, h);

    /* M4 = A22(B21 − B11) */
    subMatrix(B21, B11, T1, h);
    strassen_recursive(A22, T1, M4, h);

    /* M5 = (A11 + A12)B22 */
    addMatrix(A11, A12, T1, h);
    strassen_recursive(T1, B22, M5, h);

    /* M6 = (A21 − A11)(B11 + B12) */
    // T1 = (A21 - A11)
    // T2 = (B11 + B12)
    subMatrix(A21, A11, T1, h);
    addMatrix(B11, B12, T2, h);
    strassen_recursive(T1, T2, M6, h);

    /* M7 = (A12 − A22)(B21 + B22) */
    // T1 = (A12 - A22)
    // T2 = (B21 + B22)
    subMatrix(A12, A22, T1, h);
    addMatrix(B21, B22, T2, h);
    strassen_recursive(T1, T2, M7, h);

    // Cuadrantes de la matriz resultado C
    double* C11 = allocMatrix(h);
    double* C12 = allocMatrix(h);
    double* C21 = allocMatrix(h);
    double* C22 = allocMatrix(h);

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

    // Copiar los 4 cuadrantes de vuelta a la matriz resultado C:
    //   C = | C11  C12 |
    //       | C21  C22 |

    for (int i = 0; i < h; i++) {
        memcpy(C + i*n,         C11 + i*h, h * sizeof(double));
        memcpy(C + i*n + h,     C12 + i*h, h * sizeof(double));
        memcpy(C + (i+h)*n,     C21 + i*h, h * sizeof(double));
        memcpy(C + (i+h)*n + h, C22 + i*h, h * sizeof(double));
    }

    // Liberar toda la memoria auxiliar de los cuadrantes y productos temporales
    freeMatrix(A11);
    freeMatrix(A12);
    freeMatrix(A21);
    freeMatrix(A22);
    freeMatrix(B11);
    freeMatrix(B12);
    freeMatrix(B21);
    freeMatrix(B22);
    freeMatrix(M1);
    freeMatrix(M2);
    freeMatrix(M3);
    freeMatrix(M4);
    freeMatrix(M5);
    freeMatrix(M6);
    freeMatrix(M7);
    freeMatrix(T1);
    freeMatrix(T2);
    freeMatrix(C11);
    freeMatrix(C12);
    freeMatrix(C21);
    freeMatrix(C22);
}

// Si n no es potencia de 2, se aplica padding (relleno con ceros) hasta
// la siguiente potencia de 2, se ejecuta el algoritmo, y se recorta el
// resultado al tamaño original.
void strassen(double* A, double* B, double* C, int n) {
    int np2 = next_power_of_2(n);

    // Si ya es potencia de 2, ejecutar directamente sin overhead de padding
    if (np2 == n) {
        strassen_recursive(A, B, C, n);
        return;
    }

    // Asignar matrices ampliadas al tamaño potencia de 2
    double* A_ampliada = allocMatrix(np2);
    double* B_ampliada = allocMatrix(np2);
    double* C_ampliada = allocMatrix(np2);

    // Inicializar con ceros (las zonas de padding quedarán en 0)
    zeroMatrix(A_ampliada, np2);
    zeroMatrix(B_ampliada, np2);

    // Copiar el contenido original de A y B en la esquina superior izquierda
    for (int i = 0; i < n; i++) {
        memcpy(A_ampliada + i*np2, A + i*n, n * sizeof(double));
        memcpy(B_ampliada + i*np2, B + i*n, n * sizeof(double));
    }

    // Ejecutar Strassen sobre las matrices ampliadas
    strassen_recursive(A_ampliada, B_ampliada, C_ampliada, np2);

    // Recortar el resultado: copiar solo la submatriz n×n original
    for (int i = 0; i < n; i++) {
        memcpy(C + i*n, C_ampliada + i*np2, n * sizeof(double));
    }

    // Liberar las matrices temporales de padding
    freeMatrix(A_ampliada);
    freeMatrix(B_ampliada);
    freeMatrix(C_ampliada);
}
