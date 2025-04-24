# Registro de Problemas y Soluciones

## 1. Múltiples Formatos de Archivo de Máscara

### Problema
Los archivos de máscara venían en dos formatos diferentes:
- Caso 0/1: Pares de valores (val1, val2)
- Caso 2: Tripletas RGB directas

### Solución
- Implementación de detección automática de formato
- Lectura adaptativa según el formato detectado
- Validación de datos para ambos formatos

## 2. Número de Píxeles Incorrecto

### Problema
El número de píxeles declarado en la primera línea no coincidía con el contenido real del archivo en el Caso 2.

### Solución
- Conteo real de líneas en el archivo
- Uso del número real de líneas en lugar del declarado
- Validación de datos suficientes

## 3. Manejo de Memoria

### Problema
Posibles fugas de memoria al procesar múltiples archivos y manejar errores.

### Solución
- Implementación de limpieza de recursos en casos de error
- Uso de punteros para gestión eficiente de memoria
- Validación de asignación exitosa de memoria

## 4. Compatibilidad de Formatos

### Problema
Diferentes formatos de imagen y codificación de colores entre archivos.

### Solución
- Estandarización a formato RGB
- Conversión automática de formatos
- Validación de compatibilidad

## 5. Lectura de Archivos

### Problema
Dificultades con rutas de archivo y acceso a directorios.

### Solución
- Implementación de manejo de rutas relativas y absolutas
- Validación de existencia de archivos
- Mensajes de error descriptivos

## 6. Optimización de Rendimiento

### Problema
Procesamiento lento con imágenes grandes y múltiples archivos de máscara.

### Solución
- Optimización de algoritmos de lectura
- Mejora en el manejo de memoria
- Reducción de operaciones redundantes

## 7. Validación de Datos

### Problema
Datos incorrectos o corruptos en archivos de entrada.

### Solución
- Implementación de validaciones exhaustivas
- Mensajes de error específicos
- Manejo gracioso de errores

## 8. Compatibilidad entre Casos

### Problema
Necesidad de mantener compatibilidad con todos los casos de prueba.

### Solución
- Diseño flexible del sistema
- Pruebas con múltiples casos
- Documentación de formatos soportados

## Lecciones Aprendidas

1. Importancia de la validación de datos de entrada
2. Necesidad de manejo robusto de errores
3. Beneficios de la detección automática de formatos
4. Valor de la documentación clara
5. Importancia de la optimización temprana 