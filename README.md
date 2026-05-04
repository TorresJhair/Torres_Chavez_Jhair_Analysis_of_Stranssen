# Torres_Chavez_Jhair_Analysis_of_Stranssen

## Experimentos

### 1. Prueba con matrices no potencia de 2

Se añadieron pruebas de multiplicación de matrices 5x5 tanto en `test_mult_classic.c` como en `test_strassen.c` para comparar el comportamiento de ambos algoritmos con un tamaño que no es potencia de 2.

#### Resultados

- **`mult_classic` (5x5)**: La prueba pasó exitosamente. El algoritmo clásico de multiplicación de matrices funciona correctamente con cualquier tamaño n×n, ya que se basa en tres bucles anidados que iteran independientemente de si n es potencia de 2 o no.

- **`strassen` (5x5)**: La prueba falló con `Assertion failed` en `test_strassen.c:121`. Esto ocurre porque el algoritmo de Strassen está diseñado recursivamente dividiendo las matrices en 4 subcuadrantes de tamaño n/2×n/2, lo cual requiere implícitamente que n sea una potencia de 2. Cuando n=5, las divisiones sucesivas (5→2→1) no producen submatrices de tamaño uniforme, generando resultados incorrectos al comparar contra la multiplicación clásica.

#### Solución

Se separó la lógica en una función recursiva y un wrapper público para manejar el padding de matrices que no son potencia de 2. El proceso consiste en elevar el orden de la matriz al siguiente $2^k$ rellenando con ceros, ejecutar el algoritmo de Strassen y, finalmente, recortar el resultado al tamaño original. Esto permite que el algoritmo sea universal sin alterar la precisión de los cálculos.

### 2. Comparación de rendimiento (benchmark)

Se realizó un experimento midiendo el tiempo de ejecución de ambos algoritmos para matrices de tamaño $n \times n$ con $n$ desde **2 hasta 256** con **paso de 2**. Los tiempos se midieron con `CLOCK_MONOTONIC` para alta precisión y se almacenaron en `build/results.txt`.

#### Resultados

Los datos obtenidos en `results.txt` muestran que **Strassen no supera al algoritmo clásico** en ningún punto del rango evaluado. De hecho, ocurre lo contrario:

- Para valores pequeños ($n < 16$), Strassen es unas pocas veces más lento.
- A medida que $n$ crece, la brecha se amplía drásticamente.
- Cerca de $n = 256$, Strassen llega a ser **aproximadamente 100 veces más lento** que `mult_classic`.

#### Análisis

Esto se explica por los siguientes factores:

1. **Overhead de recursión y memoria**: Strassen crea y libera múltiples submatrices en cada nivel de recursión (cuadrantes, productos M1–M7, temporales). Este manejo de memoria dinámica tiene un costo significativo.

2. **Padding constante**: Para tamaños que no son potencia de 2, se añade overhead adicional al crear matrices ampliadas, copiar datos y recortar resultados.

3. **Cache y localidad**: `mult_classic` usa un orden de bucles i-k-j que aprovecha mejor la caché del CPU, mientras que Strassen tiene un patrón de acceso a memoria más disperso.