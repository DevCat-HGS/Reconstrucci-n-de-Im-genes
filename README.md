# Proyecto de Reconstrucción de Imágenes

Este proyecto implementa un sistema de reconstrucción de imágenes utilizando C++ y el framework Qt. El sistema es capaz de procesar diferentes formatos de archivos de máscara y reconstruir imágenes basándose en múltiples imágenes de entrada.

## Estructura del Proyecto

- `docs/`: Documentación detallada del proyecto
  - `analisis.md`: Análisis detallado del problema
  - `algoritmos.md`: Documentación de algoritmos implementados
  - `problemas.md`: Registro de problemas y soluciones
  - `evolucion.md`: Evolución de la solución
  - `diagramas.md`: Diagramas de flujo y esquemas

## Requisitos

- Qt Framework
- Compilador C++ compatible
- Sistema operativo: Windows/Linux/MacOS

## Compilación

1. Abrir el proyecto en Qt Creator
2. Configurar el kit de compilación
3. Compilar el proyecto

## Uso

El programa acepta como entrada:
- Imagen máscara (M.bmp)
- Imagen objetivo (I_O.bmp)
- Imagen donante (I_D.bmp)
- Archivos de máscara (M0.txt, M1.txt, etc.)

## Estructura de Archivos

```
├── main.cpp
├── image_utils.cpp
├── image_utils.h
└── Casos/
    ├── Caso 0/
    ├── Caso 1/
    └── Caso 2/
``` 