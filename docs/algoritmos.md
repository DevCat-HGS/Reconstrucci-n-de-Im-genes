# Documentación de Algoritmos Implementados

## 1. Carga de Imágenes (loadPixels)

```cpp
unsigned char* loadPixels(QString input, int &width, int &height)
```

### Descripción
Carga una imagen BMP y la convierte en un arreglo de píxeles RGB.

### Proceso
1. Carga la imagen usando QImage
2. Obtiene dimensiones (ancho y alto)
3. Reserva memoria dinámica para el arreglo de píxeles
4. Convierte cada píxel a formato RGB
5. Retorna el arreglo de píxeles

### Complejidad
- Tiempo: O(width * height)
- Espacio: O(width * height * 3)

## 2. Carga de Máscaras (loadSeedMasking)

```cpp
unsigned int* loadSeedMasking(const char* nombreArchivo, int &seed, int &n_pixels)
```

### Descripción
Lee archivos de máscara en múltiples formatos y los convierte en datos RGB.

### Proceso
1. Abre el archivo de máscara
2. Lee el número de píxeles
3. Determina el formato (2 o 3 valores por línea)
4. Procesa los datos según el formato:
   - Formato 1: Convierte pares en tripletas RGB
   - Formato 2: Lee tripletas RGB directamente
5. Retorna el arreglo de datos RGB

### Complejidad
- Tiempo: O(n_pixels)
- Espacio: O(n_pixels * 3)

## 3. Exportación de Imagen (exportImage)

```cpp
bool exportImage(unsigned char* pixelData, int width, int height, QString archivoSalida)
```

### Descripción
Convierte un arreglo de píxeles RGB en una imagen BMP.

### Proceso
1. Valida los datos de entrada
2. Crea una nueva imagen QImage
3. Copia los datos RGB al formato de imagen
4. Guarda la imagen en disco

### Complejidad
- Tiempo: O(width * height)
- Espacio: O(1) (usa la memoria existente)

## 4. Reconstrucción de Imagen

### Descripción
Proceso principal que combina las imágenes usando los datos de máscara.

### Proceso
1. Carga todas las imágenes necesarias
2. Para cada archivo de máscara:
   - Lee los datos de máscara
   - Aplica los cambios a la imagen objetivo
   - Actualiza la imagen en proceso
3. Guarda la imagen final

### Optimizaciones
- Lectura eficiente de archivos
- Manejo de memoria dinámica
- Validación de datos de entrada

## 5. Manejo de Errores

### Validaciones Implementadas
- Existencia de archivos
- Formato de datos válido
- Dimensiones de imagen correctas
- Memoria suficiente
- Datos de máscara válidos

### Respuesta a Errores
- Mensajes descriptivos
- Limpieza de recursos
- Estado de error apropiado 