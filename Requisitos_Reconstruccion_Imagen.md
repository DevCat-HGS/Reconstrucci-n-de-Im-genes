# Especificaciones para la Reconstrucción de Imágenes

La empresa Informa2 necesita desarrollar un programa capaz de reconstruir una imagen original a partir de la siguiente información:

## Insumos Proporcionados

* Una imagen I<sub>D</sub> de dimensiones *m* filas por *n* columnas, con tres canales (RGB), que representa el resultado final tras haber sido sometida a una serie de transformaciones a nivel de bits, aplicadas en un orden desconocido.

* Una imagen I<sub>M</sub>, también de *m* filas por *n* columnas y tres canales (RGB), generada aleatoriamente, que pudo haber sido utilizada en una o varias ocasiones durante el proceso de transformación mediante operaciones XOR con versiones intermedias de I<sub>D</sub>.

* Una máscara M, de dimensiones *i ≤ m* filas y *j ≤ n* columnas, con tres canales (RGB), utilizada para aplicar un enmascaramiento después de cada transformación a nivel de bits. Este enmascaramiento consiste en seleccionar aleatoriamente un píxel de desplazamiento s en la imagen transformada I<sub>D</sub>, y a partir de este calcular las sumas:

  S(k) = ID(k + s) + M(k)  para  0 ≤ k < i × j × 3

* *N* archivos .txt, que contienen la información generada durante el enmascaramiento aplicado en cada etapa del proceso.

  Cada archivo incluye:
  
  - En la primera línea, un valor entero que representa el desplazamiento s utilizado para aplicar el enmascaramiento.
  - En las líneas siguientes, conjuntos de tres valores enteros que representan la suma de los canales RGB, píxel a píxel, entre una porción transformada de I<sub>D</sub> y la máscara M. La Figura 2 muestra un ejemplo de las primeras siete filas de un archivo resultado que almacena el resultado del enmascaramiento.

### Ejemplo de archivo de enmascaramiento:

```
100
303 303 423
423 423 275
275 275 295
295 295 408
153 153 145
145 145 245
```

*Figura 2. Resultado del enmascaramiento usando una semilla de 100 (Primeras 7 filas)*

## Insumos Proporcionados para el Desarrollo

Como insumo, se le proporciona:

1. Código fuente de un programa que abre una imagen BMP y permite acceder a los valores de los píxeles, de forma que pueda operar con dicha información usando arreglos dinámicos.

2. Código fuente para abrir los archivos que contienen el resultado del enmascaramiento, permitiéndole operar dichos valores con arreglos dinámicos.

3. Código fuente para exportar la información contenida en arreglos dinámicos como imágenes.

4. Dos conjuntos de archivos que representan dos posibles entradas y salidas del sistema.