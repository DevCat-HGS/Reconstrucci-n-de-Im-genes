/*
 * Programa para reconstrucción de imágenes
 * Basado en los requisitos proporcionados en Requisitos_Reconstruccion_Imagen.md
 *
 * Este programa reutiliza las funciones de manejo de archivos del código base proporcionado
 * y las adapta para trabajar exclusivamente con punteros y memoria dinámica, eliminando el uso
 * de estructuras y STL.
 */

#include <iostream>
#include <QCoreApplication>
#include <QImage>
#include <QString>
#include <direct.h>   // Para _mkdir
#include <io.h>       // Para _access
#define WIN32_LEAN_AND_MEAN
#include <windows.h>  // Para Windows API
#include "image_utils.h"
using namespace std;

// Función para aplicar XOR entre dos arrays de píxeles
void aplicarXOR(unsigned char* destino, const unsigned char* img1, const unsigned char* img2, int totalPixels) {
    cout << "\nIniciando operación XOR entre imágenes..." << endl;
    cout << "Total de píxeles a procesar: " << totalPixels << endl;
    for(int i = 0; i < totalPixels * 3; i++) {
        if(i < 10) { // Mostrar solo los primeros 10 valores para no saturar la salida
            cout << "Debug: XOR - Valor1: " << (int)img1[i] << ", Valor2: " << (int)img2[i];
            destino[i] = img1[i] ^ img2[i];
            cout << ", Resultado: " << (int)destino[i] << endl;
        } else {
            destino[i] = img1[i] ^ img2[i];
        }
    }
    cout << "Debug: Operación XOR completada" << endl;
}

// Función para sumar dos arrays de píxeles
void sumarPixeles(unsigned char* destino, const unsigned char* img1, const unsigned char* img2, int totalPixels) {
    cout << "\nIniciando suma de píxeles entre imágenes..." << endl;
    cout << "Total de píxeles a procesar: " << totalPixels << endl;
    for(int i = 0; i < totalPixels * 3; i++) {
        destino[i] = img1[i] + img2[i];
    }
}

// Función para restar dos arrays de píxeles
void restarPixeles(unsigned char* destino, const unsigned char* img1, const unsigned char* img2, int totalPixels) {
    cout << "\nIniciando resta de píxeles entre imágenes..." << endl;
    cout << "Total de píxeles a procesar: " << totalPixels << endl;
    for(int i = 0; i < totalPixels * 3; i++) {
        destino[i] = img1[i] - img2[i];
    }
}

// Función para desplazamiento a la izquierda de bits
void desplazarIzquierda(unsigned char* destino, const unsigned char* img, int totalPixels, int bits) {
    bits = bits % 8; // Asegurar que el desplazamiento sea menor a 8 bits
    cout << "\nIniciando desplazamiento a la izquierda..." << endl;
    cout << "Bits a desplazar: " << bits << endl;
    cout << "Total de píxeles a procesar: " << totalPixels << endl;
    for(int i = 0; i < totalPixels * 3; i++) {
        if(i < 10) { // Mostrar solo los primeros 10 valores
            cout << "Debug: Desplazamiento Izq - Valor original: " << (int)img[i];
            destino[i] = img[i] << bits;
            cout << ", Resultado: " << (int)destino[i] << endl;
        } else {
            destino[i] = img[i] << bits;
        }
    }
    cout << "Debug: Desplazamiento a la izquierda completado" << endl;
}

// Función para desplazamiento a la derecha de bits
void desplazarDerecha(unsigned char* destino, const unsigned char* img, int totalPixels, int bits) {
    bits = bits % 8; // Asegurar que el desplazamiento sea menor a 8 bits
    cout << "\nIniciando desplazamiento a la derecha..." << endl;
    cout << "Bits a desplazar: " << bits << endl;
    cout << "Total de píxeles a procesar: " << totalPixels << endl;
    for(int i = 0; i < totalPixels * 3; i++) {
        if(i < 10) { // Mostrar solo los primeros 10 valores
            cout << "Debug: Desplazamiento Der - Valor original: " << (int)img[i];
            destino[i] = img[i] >> bits;
            cout << ", Resultado: " << (int)destino[i] << endl;
        } else {
            destino[i] = img[i] >> bits;
        }
    }
    cout << "Debug: Desplazamiento a la derecha completado" << endl;
}

// Función para rotación a la izquierda de bits
void rotarIzquierda(unsigned char* destino, const unsigned char* img, int totalPixels, int bits) {
    bits = bits % 8; // Asegurar que la rotación sea menor a 8 bits
    cout << "\nIniciando rotación a la izquierda..." << endl;
    cout << "Bits a rotar: " << bits << endl;
    cout << "Total de píxeles a procesar: " << totalPixels << endl;
    for(int i = 0; i < totalPixels * 3; i++) {
        if(i < 10) { // Mostrar solo los primeros 10 valores
            cout << "Debug: Rotación Izq - Valor original: " << (int)img[i];
            destino[i] = (img[i] << bits) | (img[i] >> (8 - bits));
            cout << ", Resultado: " << (int)destino[i] << endl;
        } else {
            destino[i] = (img[i] << bits) | (img[i] >> (8 - bits));
        }
    }
    cout << "Debug: Rotación a la izquierda completada" << endl;
}

// Función para rotación a la derecha de bits
void rotarDerecha(unsigned char* destino, const unsigned char* img, int totalPixels, int bits) {
    bits = bits % 8; // Asegurar que la rotación sea menor a 8 bits
    cout << "\nIniciando rotación a la derecha..." << endl;
    cout << "Bits a rotar: " << bits << endl;
    cout << "Total de píxeles a procesar: " << totalPixels << endl;
    for(int i = 0; i < totalPixels * 3; i++) {
        if(i < 10) { // Mostrar solo los primeros 10 valores
            cout << "Debug: Rotación Der - Valor original: " << (int)img[i];
            destino[i] = (img[i] >> bits) | (img[i] << (8 - bits));
            cout << ", Resultado: " << (int)destino[i] << endl;
        } else {
            destino[i] = (img[i] >> bits) | (img[i] << (8 - bits));
        }
    }
    cout << "Debug: Rotación a la derecha completada" << endl;
}

// Función para cargar una imagen BMP usando memoria dinámica
unsigned char* cargarImagenBMP(const char* ruta, int& ancho, int& alto) {
    QString rutaQt = QString::fromLocal8Bit(ruta);
    cout << "Cargando imagen BMP desde: " << ruta << endl;
    return loadPixels(rutaQt, ancho, alto);
}

// Función para guardar una imagen BMP usando memoria dinámica
bool guardarImagenBMP(const unsigned char* pixelData, int ancho, int alto, const char* ruta) {
    QString rutaQt = QString::fromLocal8Bit(ruta);
    cout << "Guardando imagen BMP en: " << ruta << endl;
    return exportImage(const_cast<unsigned char*>(pixelData), ancho, alto, rutaQt);
}

// Función para reconstruir la imagen original usando memoria dinámica
unsigned char* reconstruirImagen(const unsigned char* imagenTransformada, const unsigned char* /*imagenAleatoria*/,
                                 const unsigned char* mascara, int anchoTransf, int altoTransf,
                                 int anchoMascara, int altoMascara, const char* rutaMascara,
                                 const unsigned char* imagenD = nullptr) {
    if (!imagenTransformada || anchoTransf <= 0 || altoTransf <= 0) {
        cerr << "Error: La imagen transformada no es válida" << endl;
        return nullptr;
    }

    // Reservar memoria para la imagen actual
    int totalPixels = anchoTransf * altoTransf;
    unsigned char* imagenActual = new unsigned char[totalPixels * 3];

    // Copiar la imagen transformada inicial
    memcpy(imagenActual, imagenTransformada, totalPixels * 3);

    // Cargar datos de enmascaramiento
    int seed = 0;
    int n_pixels = 0;
    unsigned int* maskingData = loadSeedMasking(rutaMascara, seed, n_pixels);

    if (!maskingData) {
        cerr << "Error al cargar datos de enmascaramiento" << endl;
        delete[] imagenActual;
        return nullptr;
    }

    // Procesar la imagen
    for (int i = 0; i < n_pixels && i < (anchoMascara * altoMascara); i++) {
        int pixelDesplazadoIdx = seed + i;
        if (pixelDesplazadoIdx >= totalPixels) continue;

        // Calcular coordenadas del píxel desplazado
        int pixelDesplazadoX = pixelDesplazadoIdx % anchoTransf;
        int pixelDesplazadoY = pixelDesplazadoIdx / anchoTransf;
        int pixelDesplazadoOffset = (pixelDesplazadoY * anchoTransf + pixelDesplazadoX) * 3;

        // Calcular coordenadas en la máscara
        int mascaraX = i % anchoMascara;
        int mascaraY = i / anchoMascara;
        int mascaraOffset = (mascaraY * anchoMascara + mascaraX) * 3;

        // Aplicar los valores de la máscara directamente
        for (int c = 0; c < 3; c++) {
            imagenActual[pixelDesplazadoOffset + c] =
                static_cast<unsigned char>(maskingData[i * 3 + c] - mascara[mascaraOffset + c]);
        }
    }

    // Si existe la imagen I_D, aplicar operación adicional
    if (imagenD) {
        // Crear buffer temporal para el resultado
        unsigned char* tempBuffer = new unsigned char[totalPixels * 3];

        // Aplicar XOR con I_D
        aplicarXOR(tempBuffer, imagenActual, imagenD, totalPixels);

        // Copiar resultado al buffer principal
        memcpy(imagenActual, tempBuffer, totalPixels * 3);

        // Liberar buffer temporal
        delete[] tempBuffer;
    }

    // Guardar imagen intermedia
    guardarImagenBMP(imagenActual, anchoTransf, altoTransf, "paso_reconstruido.bmp");

    // Liberar memoria
    delete[] maskingData;

    return imagenActual;
}

int main(int argc, char* argv[]) {
    try {
        cout << "Programa de Reconstrucción de Imágenes" << endl;
        cout << "======================================" << endl;

        const char* dirCasoDefault = "C:\\Users\\ADMIN\\Documents\\untitled1\\";

        // Si no hay argumentos, usar el directorio por defecto
        const char* directorioEntrada = (argc < 2) ? dirCasoDefault : argv[1];

        // Construir rutas basadas en el directorio del caso
        char dirCaso[MAX_PATH];
        strcpy(dirCaso, directorioEntrada);
        size_t len = strlen(dirCaso);
        if (len > 0 && dirCaso[len-1] != '/' && dirCaso[len-1] != '\\') {
            dirCaso[len] = '/';
            dirCaso[len + 1] = '\0';
        }

        // Función para encontrar archivos de máscara
        WIN32_FIND_DATAA findData;
        char patronBusqueda[MAX_PATH];
        sprintf(patronBusqueda, "%sM*.txt", dirCaso);
        HANDLE hFind = FindFirstFileA(patronBusqueda, &findData);

        if (hFind == INVALID_HANDLE_VALUE) {
            cerr << "Error: No se encontraron archivos de máscara M*.txt" << endl;
            return 1;
        }

        // Rutas de los archivos base
        char rutaImagenTransformada[MAX_PATH];
        char rutaMascara[MAX_PATH];
        char rutaImagenD[MAX_PATH];

        sprintf(rutaImagenTransformada, "%sI_M.bmp", dirCaso);
        sprintf(rutaMascara, "%sM.bmp", dirCaso);
        sprintf(rutaImagenD, "%sI_D.bmp", dirCaso);

        // Verificar si existe el archivo I_D.bmp
        bool tieneImagenD = (_access(rutaImagenD, 0) != -1);

        // Verificar que las rutas se hayan generado correctamente
        cout << "Rutas de archivos:" << endl;
        cout << "I_M.bmp: " << rutaImagenTransformada << endl;
        cout << "M.bmp: " << rutaMascara << endl;
        if (tieneImagenD) {
            cout << "I_D.bmp: " << rutaImagenD << endl;
        }

        // Vector dinámico para almacenar las rutas de los archivos de máscara
        char** rutasMascaras = nullptr;
        int numMascaras = 0;

        // Contar y almacenar las rutas de los archivos de máscara
        do {
            if (strstr(findData.cFileName, ".txt")) {
                char** temp = new char*[numMascaras + 1];
                for(int i = 0; i < numMascaras; i++) {
                    temp[i] = rutasMascaras[i];
                }
                temp[numMascaras] = new char[MAX_PATH];
                sprintf(temp[numMascaras], "%s%s", dirCaso, findData.cFileName);

                if (rutasMascaras) {
                    delete[] rutasMascaras;
                }
                rutasMascaras = temp;
                numMascaras++;
            }
        } while (FindNextFileA(hFind, &findData));

        FindClose(hFind);

        // Variables para almacenar dimensiones
        int anchoTransf = 0, altoTransf = 0;
        int anchoMasc = 0, altoMasc = 0;
        int anchoD = 0, altoD = 0;
        unsigned char* imagenD = nullptr;

        // Cargar imágenes usando las funciones del código base
        cout << "Cargando imágenes..." << endl;
        unsigned char* imagenTransformada = cargarImagenBMP(rutaImagenTransformada, anchoTransf, altoTransf);
        unsigned char* mascara = cargarImagenBMP(rutaMascara, anchoMasc, altoMasc);

        // Cargar I_D.bmp si existe
        if (tieneImagenD) {
            cout << "Cargando imagen I_D.bmp..." << endl;
            imagenD = cargarImagenBMP(rutaImagenD, anchoD, altoD);
            if (!imagenD) {
                cerr << "Error: No se pudo cargar I_D.bmp" << endl;
                tieneImagenD = false;
            } else if (anchoD != anchoTransf || altoD != altoTransf) {
                cerr << "Error: Las dimensiones de I_D.bmp no coinciden" << endl;
                delete[] imagenD;
                imagenD = nullptr;
                tieneImagenD = false;
            }
        }

        // Validar que las imágenes se cargaron correctamente
        if (!imagenTransformada || !mascara) {
            cerr << "Error al cargar una o más imágenes" << endl;
            return 1;
        }

        // Procesar cada archivo de máscara
        for(int i = 0; i < numMascaras; i++) {
            cout << "\nProcesando máscara " << rutasMascaras[i] << endl;

            // Reconstruir la imagen
            cout << "Reconstruyendo imagen..." << endl;
            unsigned char* imagenReconstruida = reconstruirImagen(
                imagenTransformada, nullptr, mascara,
                anchoTransf, altoTransf, anchoMasc, altoMasc,
                rutasMascaras[i],
                tieneImagenD ? imagenD : nullptr
                );

            if (!imagenReconstruida) {
                cerr << "Error durante la reconstrucción de la imagen" << endl;
                continue;
            }

            // Generar nombre de archivo de salida
            char nombreSalida[MAX_PATH];
            sprintf(nombreSalida, "%sP%d.bmp", dirCaso, i + 1);

            // Guardar la imagen reconstruida
            cout << "Guardando imagen reconstruida..." << endl;
            bool exitoGuardado = guardarImagenBMP(imagenReconstruida, anchoTransf, altoTransf, nombreSalida);

            if (!exitoGuardado) {
                cerr << "Error al guardar la imagen reconstruida" << endl;
            } else {
                cout << "Imagen reconstruida guardada exitosamente en: " << nombreSalida << endl;
            }

            delete[] imagenReconstruida;
        }

        // Liberar memoria
        for(int i = 0; i < numMascaras; i++) {
            delete[] rutasMascaras[i];
        }
        delete[] rutasMascaras;
        delete[] imagenTransformada;
        delete[] mascara;
        if (imagenD) {
            delete[] imagenD;
        }

        cout << "\nPresiona Enter para cerrar..." << endl;
        std::cin.get();

        return 0;

    } catch (const std::exception& e) {
        cerr << "Error: " << e.what() << endl;
        cout << "\nPresiona Enter para cerrar..." << endl;
        std::cin.get();
        return 1;
    }
}
