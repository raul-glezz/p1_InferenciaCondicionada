# Informe de Análisis de Rendimiento
## Motor de Inferencia Condicional para Distribuciones Binarias

**Fecha:** 11 de febrero de 2026  
**Autores:** Raúl Gonzalez Acosta, Enrique Gómez Díaz  
**Universidad de La Laguna - Inteligencia Artificial Avanzada**

---

## 1. Enlace al repositorio 
[Repositorio del Proyecto en GitHub](https://github.com/raul-glezz/p1_InferenciaCondicionada.git)

## 2. Ejemplo de ejecución
```
enrique@MacBook-Air-de-Enrique-3 p1_InferenciaCondicionada % ./p1_InferenciaCondicionada
1. Cargar o Generar Distribución
2. Mostrar Distribución Actual
3. Ejecutar Inferencia Condicional
4. Análisis de Rendimiento
5. Exportar Distribución a CSV
6. Ayuda
0. Salir
Seleccione una opción [0-7]: 1

1. Cargar desde archivo CSV
2. Generar distribución aleatoria
Seleccione opción [1-2]: 2

Número de variables (1-20) [1-20]: 3
  Variables: 3
  Estados: 8

Pulse Enter para continuar...
1. Cargar o Generar Distribución
2. Mostrar Distribución Actual
3. Ejecutar Inferencia Condicional
4. Análisis de Rendimiento
5. Exportar Distribución a CSV
6. Ayuda
0. Salir
Seleccione una opción [0-7]: 3


--- Variables de interés ---
Introduzca el número de variables de interés:  [1-2]: 1
  Índice de variable (1-3) [1-3]: 1

--- Variables condicionadas ---
¿Agregar variables condicionadas? (s/n): s
Introduzca el número de variables condicionadas:  [0-2]: 2
  Índice de variable (1-3) [1-3]: 2
  Valor (0 o 1) [0-1]: 0
  Índice de variable (1-3) [1-3]: 3
  Valor (0 o 1) [0-1]: 1

Consulta: P(X1 | X2=0, X3=1)
  maskC: 110
  valC:  100
  maskI: 001
  maskM: 000
=== Distribución Binaria (N=1) ===
Config   | X1 | Probabilidad
----------------------------
       0 |  0 |   0.387976
       1 |  1 |   0.612024

Suma total de probabilidades: 1.000000

--- Métricas de ejecución ---
  Tiempo de ejecución: 11.42 μs
  Estados evaluados: 8
  Variables de interés: 1
  Variables condicionadas: 2
  Variables marginalizadas: 0

¿Guardar resultado en CSV? (s/n):
```

## 3. Archivo CSV de ejemplo

| Configuración | Probabilidad |
|---------------|--------------|
| 0000 | 0.06 |
| 0001 | 0.08 |
| 0010 | 0.05 |
| 0011 | 0.07 |
| 0100 | 0.09 |
| 0101 | 0.04 |
| 0110 | 0.03 |
| 0111 | 0.11 |
| 1000 | 0.10 |
| 1001 | 0.02 |
| 1010 | 0.06 |
| 1011 | 0.08 |
| 1100 | 0.05 |
| 1101 | 0.07 |
| 1110 | 0.09 |
| 1111 | 0.10 |

### Resultados:
p(x1=0) = 0.4818181818
p(x2=1|x1=0) = 0.4339622642
p(x3=0|x1=0,x2=1) = 0.4782608696
p(x4=1|x1=0,x2=1,x3=0) = 0.5454545455

## 4. Análisis

Este informe presenta un análisis exhaustivo del rendimiento del motor de inferencia condicional implementado para calcular probabilidades condicionales en distribuciones binarias discretas. Se realizaron **1,800 mediciones** sobre una distribución de **20 variables**, variando el número de variables de interés (1-10) y variables condicionadas (0-8).

### Hallazgos Principales

- **Tiempo promedio de ejecución:** 18.93 ms (18,933 μs)
- **Rango de ejecución:** 1.21 ms - 128.88 ms
- **Reducción de tiempo:** Aproximadamente **50%** por cada variable condicionada adicional
- **Complejidad espacial:** $2^{20}$ = 1,048,576 estados evaluados en todos los casos

---

### Configuración del Experimento

- **Distribución base:** 20 variables binarias discretas
- **Espacio de estados:** $2^{20}$ = 1,048,576 configuraciones
- **Repeticiones por configuración:** 20 muestras
- **Variables de interés evaluadas:** 1 a 10
- **Variables condicionadas evaluadas:** 0 a 8

### Métricas Recopiladas

Para cada configuración se midió:
- Tiempo de ejecución (microsegundos)
- Número de estados evaluados
- Media, mínimo, máximo y desviación estándar

---

## Análisis de Resultados

### Estadísticas Globales

| Métrica | Valor |
|---------|-------|
| **Mediciones totales** | 1,800 |
| **Tiempo medio** | 18,933 μs |
| **Tiempo mínimo** | 1,213.17 μs |
| **Tiempo máximo** | 128,880 μs |
| **Desviación estándar** | 25,752.2 μs |

### Impacto de las Variables de Interés

#### Tabla 1: Tiempo de ejecución según variables de interés (0 condicionadas)

| Variables de Interés | Tiempo Promedio (μs) | Incremento |
|---------------------|----------------------|------------|
| 1 | 60,118 | - |
| 2 | 66,055 | +9.9% |
| 3 | 70,607 | +17.4% |
| 4 | 75,324 | +25.3% |
| 5 | 76,285 | +26.9% |
| 6 | 79,328 | +32.0% |
| 7 | 85,474 | +42.2% |
| 8 | 96,012 | +59.7% |
| 9 | 96,333 | +60.2% |
| 10 | 99,957 | +66.3% |

**Progresión del tiempo de ejecución:**

```
1 var      2 vars     3 vars     4 vars     5 vars  ...  10 vars
60.1 ms -> 66.1 ms -> 70.6 ms -> 75.3 ms -> 76.3 ms ... 100.0 ms
  (+0%)    (+9.9%)    (+17.4%)   (+25.3%)   (+26.9%)    (+66.3%)
```

**Observación:** El incremento de variables de interés aumenta el tiempo de ejecución de forma aproximadamente lineal, con un incremento del **6-7%** por variable adicional.

### Impacto de las Variables Condicionadas

#### Tabla 2: Reducción de tiempo según variables condicionadas (1 variable de interés)

| Variables Condicionadas | Tiempo (μs) | Reducción | Reducción Acumulada |
|------------------------|-------------|-----------|---------------------|
| 0 | 60,118 | - | 0% |
| 1 | 29,976 | -50.1% | 50.1% |
| 2 | 16,967 | -43.4% | 71.8% |
| 3 | 8,878 | -47.7% | 85.2% |
| 4 | 4,873 | -45.1% | 91.9% |
| 5 | 2,993 | -38.6% | 95.0% |
| 6 | 2,073 | -30.7% | 96.6% |
| 7 | 1,546 | -25.4% | 97.4% |
| 8 | 1,366 | -11.6% | 97.7% |

**Cadena de reducción del tiempo de ejecución:**

```
0 cond.     1 cond.      2 cond.      3 cond.      4 cond.
60,118 μs → 29,976 μs → 16,967 μs → 8,878 μs → 4,873 μs
            (-50.1%)    (-43.4%)    (-47.7%)    (-45.1%)

   ↓           ↓           ↓           ↓

5 cond.     6 cond.      7 cond.      8 cond.
2,993 μs → 2,073 μs → 1,546 μs → 1,366 μs
(-38.6%)    (-30.7%)    (-25.4%)    (-11.6%)
```

**Observación clave:** Cada variable condicionada adicional reduce el tiempo de ejecución aproximadamente a la **mitad**, siguiendo un patrón exponencial decreciente.

### Análisis Matricial Completo

#### Tabla 3: Matriz de tiempos de ejecución (μs)

| Interés ↓ / Condic. → | 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 |
|-----------------------|-------|-------|-------|-------|------|------|------|------|------|
| **1** | <span style="color: #d32f2f">60,118</span> | <span style="color: #ff6f00">29,976</span> | <span style="color: #f9a825">16,967</span> | <span style="color: #f9a825">8,878</span> | <span style="color: #388e3c">4,873</span> | <span style="color: #388e3c">2,993</span> | <span style="color: #388e3c">2,073</span> | <span style="color: #388e3c">1,546</span> | <span style="color: #388e3c">1,366</span> |
| **2** | <span style="color: #d32f2f">66,055</span> | <span style="color: #ff6f00">35,018</span> | <span style="color: #f9a825">18,233</span> | <span style="color: #f9a825">9,390</span> | <span style="color: #f9a825">5,143</span> | <span style="color: #388e3c">3,112</span> | <span style="color: #388e3c">2,240</span> | <span style="color: #388e3c">1,570</span> | <span style="color: #388e3c">1,353</span> |
| **3** | <span style="color: #d32f2f">70,607</span> | <span style="color: #ff6f00">35,196</span> | <span style="color: #f9a825">18,630</span> | <span style="color: #f9a825">9,742</span> | <span style="color: #f9a825">5,734</span> | <span style="color: #388e3c">3,394</span> | <span style="color: #388e3c">2,216</span> | <span style="color: #388e3c">1,621</span> | <span style="color: #388e3c">1,323</span> |
| **4** | <span style="color: #d32f2f">75,324</span> | <span style="color: #ff6f00">38,226</span> | <span style="color: #ff6f00">20,395</span> | <span style="color: #f9a825">9,715</span> | <span style="color: #f9a825">5,789</span> | <span style="color: #388e3c">3,553</span> | <span style="color: #388e3c">2,253</span> | <span style="color: #388e3c">1,755</span> | <span style="color: #388e3c">1,435</span> |
| **5** | <span style="color: #d32f2f">76,285</span> | <span style="color: #ff6f00">40,813</span> | <span style="color: #f9a825">19,346</span> | <span style="color: #f9a825">10,860</span> | <span style="color: #f9a825">6,053</span> | <span style="color: #388e3c">3,668</span> | <span style="color: #388e3c">2,339</span> | <span style="color: #388e3c">1,703</span> | <span style="color: #388e3c">1,366</span> |
| **6** | <span style="color: #d32f2f">79,328</span> | <span style="color: #ff6f00">42,003</span> | <span style="color: #ff6f00">22,643</span> | <span style="color: #f9a825">11,672</span> | <span style="color: #f9a825">6,858</span> | <span style="color: #388e3c">3,970</span> | <span style="color: #388e3c">2,604</span> | <span style="color: #388e3c">1,864</span> | <span style="color: #388e3c">1,480</span> |
| **7** | <span style="color: #d32f2f">85,474</span> | <span style="color: #ff6f00">43,182</span> | <span style="color: #ff6f00">23,177</span> | <span style="color: #f9a825">12,231</span> | <span style="color: #f9a825">6,806</span> | <span style="color: #388e3c">4,139</span> | <span style="color: #388e3c">2,738</span> | <span style="color: #388e3c">1,946</span> | <span style="color: #388e3c">1,454</span> |
| **8** | <span style="color: #d32f2f">96,012</span> | <span style="color: #ff6f00">46,965</span> | <span style="color: #ff6f00">24,636</span> | <span style="color: #f9a825">12,797</span> | <span style="color: #f9a825">6,793</span> | <span style="color: #388e3c">3,960</span> | <span style="color: #388e3c">2,481</span> | <span style="color: #388e3c">1,733</span> | <span style="color: #388e3c">1,414</span> |
| **9** | <span style="color: #d32f2f">96,333</span> | <span style="color: #ff6f00">48,276</span> | <span style="color: #ff6f00">26,098</span> | <span style="color: #f9a825">13,628</span> | <span style="color: #f9a825">7,469</span> | <span style="color: #388e3c">4,403</span> | <span style="color: #388e3c">2,725</span> | <span style="color: #388e3c">1,909</span> | <span style="color: #388e3c">1,521</span> |
| **10** | <span style="color: #d32f2f">99,957</span> | <span style="color: #d32f2f">51,802</span> | <span style="color: #ff6f00">26,919</span> | <span style="color: #f9a825">13,866</span> | <span style="color: #f9a825">7,812</span> | <span style="color: #388e3c">4,392</span> | <span style="color: #388e3c">2,739</span> | <span style="color: #388e3c">1,964</span> | <span style="color: #388e3c">1,557</span> |

**Código de colores:**
- <span style="color: #388e3c">**Verde**</span> (< 5,000 μs): Excelente rendimiento
- <span style="color: #f9a825">**Amarillo**</span> (5,000-20,000 μs): Rendimiento aceptable  
- <span style="color: #ff6f00">**Naranja**</span> (20,000-50,000 μs): Rendimiento moderado
- <span style="color: #d32f2f">**Rojo**</span> (> 50,000 μs): Alto tiempo de ejecución

### Visualización del Rendimiento

**Factores que afectan el tiempo de ejecución:**

| Factor | Efecto | Magnitud | Impacto |
|--------|--------|----------|----------|
| **Variables de Interés** | ↗ Incremento | Lineal (~7% por variable) | Moderado |
| **Variables Condicionadas** | ↘ Reducción | Exponencial (~50% por variable) | **Alto** |
| **Estados Evaluados** | — Constante | Fijo (1,048,576) | Base |

**Relación de los factores:**

- **Variables de Interés**: Incremento lineal del ~7% por variable adicional
- **Variables Condicionadas**: Reducción exponencial del ~50% por variable adicional  
- **Estados Evaluados**: Constante en 1,048,576 para todas las configuraciones

---

## Observaciones Clave

### Eficiencia del Condicionamiento

El condicionamiento de variables demuestra ser **extremadamente efectivo** para reducir el tiempo de cálculo:

- Con **4 variables condicionadas**, el tiempo se reduce al **~8%** del original
- Con **8 variables condicionadas**, el tiempo se reduce al **~2%** del original
- La reducción es consistente independientemente del número de variables de interés

### Escalabilidad

El sistema muestra patrones de escalabilidad predecibles:

1. **Complejidad por variables de interés:** $O(n)$ - lineal
2. **Reducción por condicionamiento:** $O(2^{-k})$ - exponencial decreciente
3. **Estados evaluados:** $O(2^N)$ - exponencial en el número total de variables

---

## Análisis Comparativo

### Eficiencia Relativa por Configuración

#### Configuraciones Más Eficientes (< 2,000 μs)

**Top 10 Configuraciones Óptimas**

| Interés/Cond | Tiempo (μs) | Uso Recomendado |
|:------------:|:-----------:|:---------------:|
| 1 / 8 | 1,366 | Consulta rápida |
| 3 / 8 | 1,323 | Mejor caso |
| 2 / 8 | 1,353 | Alta precisión |
| 7 / 8 | 1,454 | Análisis simple |
| 4 / 8 | 1,435 | Eficiente |
| 1 / 7 | 1,546 | Muy rápido |
| 2 / 7 | 1,570 | Balanceado |
| 3 / 7 | 1,621 | Óptimo |
| 5 / 7 | 1,703 | Alta velocidad |
| 8 / 7 | 1,733 | Equilibrado |

---

## Entrorno de Ejecución

- **Compilador:** g++ con optimización -std=c++23
- **Arquitectura:** x86_64/ARM64
- **Medición:** std::chrono::high_resolution_clock
- **Precisión:** Microsegundos (μs)

---

## Porcentajes de participación de cada miembro

| Miembro | Porcentaje de Participación |
|---------|-----------------------------|
| Raúl Gonzalez Acosta | 50% |
| Enrique Gómez Díaz | 50% |

---

*Universidad de La Laguna*  
*Escuela Superior de Ingeniería y Tecnología*  
*Grado en Ingeniería Informática*
