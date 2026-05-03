# Torres_Chavez_Jhair_Analysis_of_Stranssen

## Experimentos

Se añadieron pruebas de multiplicación de matrices 5x5 tanto en `test_mult_classic.c` como en `test_strassen.c` para comparar el comportamiento de ambos algoritmos con un tamaño que no es potencia de 2.

### Resultados

- **`mult_classic` (5x5)**: La prueba pasó exitosamente. El algoritmo clásico de multiplicación de matrices funciona correctamente con cualquier tamaño n×n, ya que se basa en tres bucles anidados que iteran independientemente de si n es potencia de 2 o no.

- **`strassen` (5x5)**: La prueba falló con `Assertion failed` en `test_strassen.c:121`. Esto ocurre porque el algoritmo de Strassen está diseñado recursivamente dividiendo las matrices en 4 subcuadrantes de tamaño n/2×n/2, lo cual requiere implícitamente que n sea una potencia de 2. Cuando n=5, las divisiones sucesivas (5→2→1) no producen submatrices de tamaño uniforme, generando resultados incorrectos al comparar contra la multiplicación clásica.

