#include <stdio.h>

// sub main tests
int test_main_mult_matrix();
int test_main_matrix();

// MAIN REAL DE TESTS
int main() {

    printf("Ejecutando tests\n");

    test_main_matrix();
    printf("\n");

    test_main_mult_matrix();
    printf("\n");

    printf("Todos tests Exitosos =)\n");

    return 0;
}