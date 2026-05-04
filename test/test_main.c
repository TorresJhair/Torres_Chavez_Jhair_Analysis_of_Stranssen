#include <stdio.h>

// sub main tests
int test_main_mult_matrix();
int test_main_matrix();
int test_main_strassen();
int test_main_benchmark();

// MAIN REAL DE TESTS
int main() {

    printf("Ejecutando tests\n");

    test_main_matrix();
    printf("\n");

    test_main_mult_matrix();
    printf("\n");

    test_main_strassen();
    printf("\n");

    test_main_benchmark();
    printf("\n");

    printf("Todos tests Exitosos =)\n");

    return 0;
}