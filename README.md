# Torres_Chavez_Jhair_Analysis_of_Stranssen

## Ejecutar el proyecto

```bash
make           # Compila y ejecuta ambas fases + genera gráficas
make run1      # Ejecuta solo fase1
make run2      # Ejecuta solo fase2
make fase1     # Compila solo fase1
make fase2     # Compila solo fase2
make tests     # Ejecuta tests con Valgrind
make clean     # Limpia archivos compilados
```

## Estructura de Fases

### Fase 1: Clásico vs Strassen MC-CUTOFF=32

Compara el algoritmo clásico de multiplicación de matrices con la implementación optimizada de Strassen que usa padding al múltiplo más cercano de 32.

- **Archivo**: `fase1.c`
- **Resultados**: `results2.txt`
- **Gráfica**: `plot_fase1.png`
- **Incluye**: Línea punteada en n₀ = 128 para referencia

### Fase 2: Comparación de estrategias de padding

Compara dos implementaciones de Strassen para analizar el impacto del método de padding.

- **Archivo**: `fase2.c`
- **Resultados**: `results.txt`
- **Gráfica**: `plot_fase2.png`
- **Incluye**:
  - **Clásico**: Multiplicación tradicional O(n³)
  - **MC-CUTOFF**: Strassen con padding al múltiplo de CUTOFF (32)
  - **Power of 2**: Strassen con padding a potencia de 2 más cercana

## Implementaciones de Strassen

| Implementación | Padding | CUTOFF | Descripción |
|----------------|---------|--------|-------------|
| `strassen_multiple_cutoff` | Múltiplo de CUTOFF | 32 | Padding eficiente, reduce memoria extra |
| `strassen_power_2` | Potencia de 2 | 32 | Padding tradicional, usa más memoria |
| `strassen` | Potencia de 2 | - | Implementación PURA recursiva sin workspace |

## Métricas de Benchmark

Los benchmarks utilizan:
- **Mediana** de 15 ejecuciones para mayor robustez
- **Desviación estándar** para analizar variabilidad
- `CLOCK_MONOTONIC` para alta precisión en mediciones

---

## Experimentos

### 1. Prueba con matrices no potencia de 2

Se añadieron pruebas de multiplicación de matrices 5x5 tanto en `test_mult_classic.c` como en `test_strassen.c` para comparar el comportamiento de ambos algoritmos con un tamaño que no es potencia de 2.

#### Resultados

- **`mult_classic` (5x5)**: La prueba pasó exitosamente. El algoritmo clásico de multiplicación de matrices funciona correctamente con cualquier tamaño n×n, ya que se basa en tres bucles anidados que iteran independientemente de si n es potencia de 2 o no.

- **`strassen` (5x5)**: La prueba falló con `Assertion failed` en `test_strassen.c:121`. Esto ocurre porque el algoritmo de Strassen está diseñado recursivamente dividiendo las matrices en 4 subcuadrantes de tamaño n/2×n/2, lo cual requiere implícitamente que n sea una potencia de 2. Cuando n=5, las divisiones sucesivas (5→2→1) no producen submatrices de tamaño uniforme, generando resultados incorrectos al comparar contra la multiplicación clásica.

#### Solución

Se separó la lógica en una función recursiva y un wrapper público para manejar el padding de matrices que no son potencia de 2. El proceso consiste en elevar el orden de la matriz al siguiente $2^k$ rellenando con ceros, ejecutar el algoritmo de Strassen y, finalmente, recortar el resultado al tamaño original. Esto permite que el algoritmo sea universal sin alterar la precisión de los cálculos.

### 2. Comparación de rendimiento

Se realizó un experimento midiendo el tiempo de ejecución de ambos algoritmos para matrices de tamaño $n \times n$ con $n$ desde **2 hasta 512** con **paso de 2**. Los tiempos se midieron con `CLOCK_MONOTONIC` para alta precisión y se almacenaron en `results.txt`.

#### Resultados

Los datos obtenidos en `results.txt` muestran que **Strassen PURO no supera al algoritmo clásico** en ningún punto del rango evaluado. De hecho, ocurre lo contrario:

- Para valores pequeños ($n < 16$), Strassen es unas pocas veces más lento.
- A medida que $n$ crece, la brecha se amplía drásticamente.
- Cerca de $n = 256$, Strassen llega a ser **aproximadamente 100 veces más lento** que `mult_classic`.

#### Análisis

Esto se explica por los siguientes factores:

1. **Overhead de recursión y memoria**: Strassen crea y libera múltiples submatrices en cada nivel de recursión (cuadrantes, productos M1–M7, temporales). Este manejo de memoria dinámica tiene un costo significativo.

2. **Padding constante**: Para tamaños que no son potencia de 2, se añade overhead adicional al crear matrices ampliadas, copiar datos y recortar resultados.

3. **Cache y localidad**: `mult_classic` usa un orden de bucles i-k-j que aprovecha mejor la caché del CPU, mientras que Strassen tiene un patrón de acceso a memoria más disperso.

#### Solución

Para mitigar estos problemas, se elaboraron implementaciones optimizadas de Strassen que incorporan:

1. **Workspace pre-allocado**: En lugar de crear y liberar múltiples submatrices en cada nivel de recursión, se reserva un bloque de memoria contiguo al inicio de la ejecución. Este workspace almacena los 7 productos intermedios (M1–M7) y las matrices temporales (T1, T2) en una única asignación, reduciendo significativamente el overhead de `malloc/free`.

3. **Acceso strided**: Las submatrices se referencian mediante punteros con stride (offset) en lugar de copiar datos.

4. **Padding inteligente**: En lugar de padding a potencia de 2 (que puede duplicar el tamaño de la matriz, ej: 257 → 512), se utiliza padding al múltiplo más cercano de CUTOFF (ej: 257 → 288), reduciendo la cantidad de elementos extra a procesar.
