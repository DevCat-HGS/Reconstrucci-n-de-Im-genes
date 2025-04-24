# Diagramas y Esquemas del Proyecto

## Diagrama de Flujo Principal

```
[Inicio] -> [Cargar Imágenes]
            |
            v
[Procesar Archivos de Máscara] -> [Para cada máscara]
                                  |
                                  v
                                [Detectar Formato]
                                  |
                                  v
                                [Leer Datos]
                                  |
                                  v
                                [Aplicar Cambios]
                                  |
                                  v
                                [Actualizar Imagen]
                                  |
                                  v
                                [¿Más máscaras?] -> [Sí] -> [Siguiente Máscara]
                                  |
                                  v
                                [No]
                                  |
                                  v
                                [Guardar Resultado]
                                  |
                                  v
                                [Fin]
```

## Estructura de Datos

```
Imagen {
    unsigned char* pixels
    int width
    int height
}

Máscara {
    unsigned int* data
    int n_pixels
    int seed
}
```

## Diagrama de Componentes

```
+----------------+     +------------------+     +----------------+
|  Carga de     |     |  Procesamiento   |     |  Exportación  |
|  Imágenes     | --> |  de Máscaras    | --> |  de Imagen    |
+----------------+     +------------------+     +----------------+
       ^                      ^
       |                      |
+----------------+     +------------------+
|  Archivos      |     |  Archivos de    |
|  de Imagen     |     |  Máscara        |
+----------------+     +------------------+
```

## Proceso de Reconstrucción

```
+-------------------+     +------------------+     +------------------+
| Imagen Original   |     | Imagen Donante   |     | Máscara         |
| (I_O.bmp)        | --> | (I_D.bmp)        | --> | (M*.txt)        |
+-------------------+     +------------------+     +------------------+
          |                      |                        |
          v                      v                        v
    +------------------------------------------------+
    |              Proceso de Reconstrucción          |
    |                                                 |
    | 1. Leer píxeles de imagen original             |
    | 2. Leer píxeles de imagen donante              |
    | 3. Aplicar máscara                             |
    | 4. Combinar píxeles según máscara              |
    +------------------------------------------------+
                           |
                           v
                  +------------------+
                  | Imagen Final     |
                  +------------------+
```

## Manejo de Memoria

```
Asignación:
[Solicitud de Memoria] -> [Verificar Disponibilidad]
                         |
                         v
                    [Asignar Memoria]
                         |
                         v
                    [Inicializar Datos]

Liberación:
[Verificar Puntero] -> [Liberar Memoria] -> [Nullificar Puntero]
```

## Flujo de Datos

```
Entrada:
[Archivos BMP] -> [Lectura] -> [Conversión a RGB] -> [Array de Píxeles]

Procesamiento:
[Array de Píxeles] -> [Aplicar Máscara] -> [Modificar Píxeles]

Salida:
[Array de Píxeles] -> [Conversión a BMP] -> [Archivo de Salida]
```

## Validación de Datos

```
[Entrada de Datos] -> [Verificar Formato]
                      |
                      v
                 [¿Formato Válido?] -> [No] -> [Error]
                      |
                      v
                    [Sí]
                      |
                      v
                 [Procesar Datos]
```

## Gestión de Errores

```
[Operación] -> [¿Error?] -> [Sí] -> [Limpiar Recursos]
                   |                      |
                   v                      v
                 [No]              [Reportar Error]
                   |                      |
                   v                      v
            [Continuar]             [Terminar]
``` 