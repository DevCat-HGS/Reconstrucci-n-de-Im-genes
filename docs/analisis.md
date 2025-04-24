# Análisis del Problema de Reconstrucción de Imágenes

## Descripción del Problema

El proyecto se centra en la reconstrucción de imágenes utilizando múltiples imágenes de entrada y archivos de máscara. El objetivo es generar una imagen final que combine elementos de una imagen objetivo y una imagen donante según las especificaciones proporcionadas en archivos de máscara.

## Componentes Principales

### 1. Imágenes de Entrada
- **Imagen Máscara (M.bmp)**: Define las regiones que se deben reconstruir
- **Imagen Objetivo (I_O.bmp)**: Imagen base que se desea modificar
- **Imagen Donante (I_D.bmp)**: Imagen fuente para la reconstrucción
- **Archivos de Máscara (M*.txt)**: Especifican los píxeles y valores para la reconstrucción

### 2. Formatos de Archivo
#### Formato Caso 0 y 1
```
n_pixels
val1 val2
val1 val2
...
```
- Primera línea: número de píxeles a procesar
- Líneas siguientes: pares de valores donde val1 se usa dos veces y val2 una vez para RGB

#### Formato Caso 2
```
total_pixels
r g b
r g b
...
```
- Primera línea: número total de píxeles
- Líneas siguientes: tripletas RGB directas

## Consideraciones Técnicas

### 1. Manejo de Memoria
- Uso de memoria dinámica para almacenar datos de imagen
- Gestión eficiente de recursos para imágenes grandes
- Liberación adecuada de memoria

### 2. Procesamiento de Archivos
- Lectura de archivos BMP
- Parsing de archivos de máscara en diferentes formatos
- Validación de datos de entrada

### 3. Reconstrucción de Imagen
- Procesamiento píxel por píxel
- Combinación de valores RGB
- Generación de imagen de salida

## Restricciones del Proyecto

1. Implementación en C++ con Qt Framework
2. No uso de STL ni estructuras de datos predefinidas
3. Uso obligatorio de punteros y memoria dinámica
4. Manejo de múltiples formatos de archivo

## Métricas de Éxito

1. Correcta lectura de todos los formatos de archivo
2. Reconstrucción precisa de imágenes
3. Manejo eficiente de memoria
4. Robustez ante diferentes casos de entrada 