// ConsoleApplication1.cpp : Programa para reconstrucción de imágenes
// Basado en los requisitos proporcionados en Requisitos_Reconstruccion_Imagen.md y referenciados en el documento DESAFIO-I.pdf

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cmath>
#include <algorithm>
#include <sstream>
#include <direct.h>   // Para _mkdir
#include <io.h>       // Para _access
#define WIN32_LEAN_AND_MEAN
#include <windows.h>  // Para Windows API

// Estructura para representar un píxel RGB
struct Pixel {
    unsigned char r, g, b;
    
    Pixel() : r(0), g(0), b(0) {}
    Pixel(unsigned char r, unsigned char g, unsigned char b) : r(r), g(g), b(b) {}
    
    // Operador XOR para píxeles
    Pixel operator^(const Pixel& other) const {
        return Pixel(r ^ other.r, g ^ other.g, b ^ other.b);
    }
    
    // Operador suma para píxeles
    Pixel operator+(const Pixel& other) const {
        return Pixel(r + other.r, g + other.g, b + other.b);
    }
    
    // Operador resta para píxeles
    Pixel operator-(const Pixel& other) const {
        return Pixel(r - other.r, g - other.g, b - other.b);
    }
};

// Estructura para representar una imagen
struct Imagen {
    int ancho, alto;
    std::vector<Pixel> pixeles;
    
    Imagen() : ancho(0), alto(0) {}
    Imagen(int ancho, int alto) : ancho(ancho), alto(alto), pixeles(ancho * alto) {}
    
    // Obtener píxel en una posición específica
    Pixel& getPixel(int x, int y) {
        if (x < 0 || x >= ancho || y < 0 || y >= alto) {
            throw std::out_of_range("Coordenadas de píxel fuera de rango");
        }
        return pixeles[y * ancho + x];
    }
    
    const Pixel& getPixel(int x, int y) const {
        if (x < 0 || x >= ancho || y < 0 || y >= alto) {
            throw std::out_of_range("Coordenadas de píxel fuera de rango");
        }
        return pixeles[y * ancho + x];
    }
    
    // Establecer píxel en una posición específica
    void setPixel(int x, int y, const Pixel& p) {
        if (x < 0 || x >= ancho || y < 0 || y >= alto) {
            throw std::out_of_range("Coordenadas de píxel fuera de rango");
        }
        pixeles[y * ancho + x] = p;
    }
    
    // Aplicar operación XOR con otra imagen
    Imagen aplicarXOR(const Imagen& otra) const {
        Imagen resultado(ancho, alto);
        for (int i = 0; i < pixeles.size(); i++) {
            resultado.pixeles[i] = pixeles[i] ^ otra.pixeles[i];
        }
        return resultado;
    }
};

// Función para cargar una imagen BMP
Imagen cargarImagenBMP(const std::string& ruta) {
    std::cout << "Cargando imagen BMP desde: " << ruta << std::endl;
    
    // Abrir el archivo en modo binario
    std::ifstream archivo(ruta, std::ios::binary);
    if (!archivo.is_open()) {
        std::cerr << "Error al abrir el archivo: " << ruta << std::endl;
        return Imagen();
    }
    
    // Leer el encabezado del archivo BMP
    unsigned char header[54];
    archivo.read(reinterpret_cast<char*>(header), 54);
    
    // Verificar que sea un archivo BMP válido
    if (header[0] != 'B' || header[1] != 'M') {
        std::cerr << "El archivo no es un BMP válido: " << ruta << std::endl;
        archivo.close();
        return Imagen();
    }
    
    // Extraer información del encabezado
    int ancho = *(int*)&header[18];
    int alto = *(int*)&header[22];
    int bitsPerPixel = *(short*)&header[28];
    
    // Verificar que sea una imagen de 24 bits (RGB)
    if (bitsPerPixel != 24) {
        std::cerr << "El archivo no es una imagen de 24 bits: " << ruta << std::endl;
        archivo.close();
        return Imagen();
    }
    
    // Calcular el padding (cada fila debe ser múltiplo de 4 bytes)
    int padding = (4 - (ancho * 3) % 4) % 4;
    
    // Crear la imagen
    Imagen img(ancho, alto);
    
    // Leer los datos de los píxeles
    // En BMP, las filas se almacenan de abajo hacia arriba
    for (int y = alto - 1; y >= 0; y--) {
        for (int x = 0; x < ancho; x++) {
            unsigned char pixel[3];
            archivo.read(reinterpret_cast<char*>(pixel), 3);
            
            // En BMP, los colores se almacenan como BGR, no RGB
            img.setPixel(x, y, Pixel(pixel[2], pixel[1], pixel[0]));
        }
        
        // Saltar el padding al final de cada fila
        archivo.seekg(padding, std::ios::cur);
    }
    
    archivo.close();
    return img;
}

// Función para guardar una imagen BMP
void guardarImagenBMP(const Imagen& img, const std::string& ruta) {
    std::cout << "Guardando imagen BMP en: " << ruta << std::endl;
    
    // Abrir el archivo en modo binario
    std::ofstream archivo(ruta, std::ios::binary);
    if (!archivo.is_open()) {
        std::cerr << "Error al crear el archivo: " << ruta << std::endl;
        return;
    }
    
    // Calcular el padding (cada fila debe ser múltiplo de 4 bytes)
    int padding = (4 - (img.ancho * 3) % 4) % 4;
    
    // Calcular el tamaño del archivo
    int dataSize = (img.ancho * 3 + padding) * img.alto;
    int fileSize = 54 + dataSize;
    
    // Crear el encabezado del archivo BMP
    unsigned char header[54] = {0};
    
    // Encabezado de archivo
    header[0] = 'B';
    header[1] = 'M';
    *(int*)&header[2] = fileSize;
    *(int*)&header[10] = 54; // Offset de los datos
    
    // Encabezado de información
    *(int*)&header[14] = 40; // Tamaño del encabezado de información
    *(int*)&header[18] = img.ancho;
    *(int*)&header[22] = img.alto;
    *(short*)&header[26] = 1; // Planos
    *(short*)&header[28] = 24; // Bits por píxel
    *(int*)&header[34] = dataSize;
    
    // Escribir el encabezado
    archivo.write(reinterpret_cast<char*>(header), 54);
    
    // Escribir los datos de los píxeles
    // En BMP, las filas se almacenan de abajo hacia arriba
    for (int y = img.alto - 1; y >= 0; y--) {
        for (int x = 0; x < img.ancho; x++) {
            Pixel p = img.getPixel(x, y);
            
            // En BMP, los colores se almacenan como BGR, no RGB
            unsigned char pixel[3] = {p.b, p.g, p.r};
            archivo.write(reinterpret_cast<char*>(pixel), 3);
        }
        
        // Agregar el padding al final de cada fila
        unsigned char paddingBytes[3] = {0, 0, 0};
        archivo.write(reinterpret_cast<char*>(paddingBytes), padding);
    }
    
    archivo.close();
}

// Estructura para almacenar información de enmascaramiento
struct DatosMascara {
    int desplazamiento;
    std::vector<std::vector<int>> sumas; // [fila][canal RGB]
    
    // Constructor para inicializar el desplazamiento
    DatosMascara() : desplazamiento(0) {}
};

// Función para cargar datos de enmascaramiento desde un archivo
DatosMascara cargarDatosMascara(const std::string& ruta) {
    DatosMascara datos;
    std::ifstream archivo(ruta);
    
    if (!archivo.is_open()) {
        std::cerr << "Error al abrir el archivo de máscara: " << ruta << std::endl;
        return datos;
    }
    
    // Leer el desplazamiento
    archivo >> datos.desplazamiento;
    
    // Leer las sumas RGB
    int r, g, b;
    while (archivo >> r >> g >> b) {
        datos.sumas.push_back({r, g, b});
    }
    
    archivo.close();
    return datos;
}

// Función para reconstruir la imagen original
Imagen reconstruirImagen(const Imagen& imagenTransformada, const Imagen& imagenAleatoria, 
                        const Imagen& mascara, const std::vector<DatosMascara>& datosMascaras) {
    if (imagenTransformada.ancho == 0 || imagenTransformada.alto == 0) {
        throw std::runtime_error("La imagen transformada no es válida");
    }

    // Comenzamos con la imagen transformada final
    Imagen imagenActual = imagenTransformada;
    
    // Procesamos cada archivo de enmascaramiento en orden inverso
    for (int i = static_cast<int>(datosMascaras.size()) - 1; i >= 0; i--) {
        const DatosMascara& datos = datosMascaras[i];
        int desplazamiento = datos.desplazamiento;
        
        std::cout << "Procesando archivo de enmascaramiento " << i + 1 << ", desplazamiento: " << desplazamiento << std::endl;
        
        // Crear una copia de la imagen actual para trabajar
        Imagen imagenAnterior(imagenActual.ancho, imagenActual.alto);
        
        // Calculamos los píxeles originales usando la fórmula inversa
        int mascaraAlto = mascara.alto;
        int mascaraAncho = mascara.ancho;
        
        // Recorremos cada píxel de la máscara
        for (int y = 0; y < mascaraAlto && y < imagenActual.alto; y++) {
            for (int x = 0; x < mascaraAncho && x < imagenActual.ancho; x++) {
                int indice = y * mascaraAncho + x;
                
                // Verificar que tenemos datos para este índice
                if (indice < 0 || static_cast<size_t>(indice) >= datos.sumas.size()) {
                    continue;  // Skip this pixel if index is out of range
                }
                
                int pixelDesplazadoIdx = desplazamiento + indice;
                int pixelDesplazadoX = pixelDesplazadoIdx % imagenActual.ancho;
                int pixelDesplazadoY = pixelDesplazadoIdx / imagenActual.ancho;
                
                // Verificar que las coordenadas están dentro de los límites
                if (pixelDesplazadoX >= 0 && pixelDesplazadoX < imagenActual.ancho &&
                    pixelDesplazadoY >= 0 && pixelDesplazadoY < imagenActual.alto &&
                    datos.sumas[indice].size() >= 3) {
                    
                    // Obtener valores de la suma S(k) del archivo de enmascaramiento
                    int sumaR = datos.sumas[indice][0];
                    int sumaG = datos.sumas[indice][1];
                    int sumaB = datos.sumas[indice][2];
                    
                    try {
                        // Obtener el píxel de la máscara M(k)
                        Pixel pixelMascara = mascara.getPixel(x, y);
                        
                        // Calcular el píxel original con manejo de desbordamiento
                        unsigned char r = static_cast<unsigned char>(sumaR - pixelMascara.r);
                        unsigned char g = static_cast<unsigned char>(sumaG - pixelMascara.g);
                        unsigned char b = static_cast<unsigned char>(sumaB - pixelMascara.b);
                        
                        // Actualizar el píxel en la imagen anterior
                        imagenAnterior.setPixel(pixelDesplazadoX, pixelDesplazadoY, Pixel(r, g, b));
                    } catch (const std::out_of_range&) {
                        std::cerr << "Error: Acceso fuera de rango en coordenadas (" << x << "," << y << ")" << std::endl;
                        continue;
                    }
                }
            }
        }
        
        // Aplicar operación XOR con la imagen aleatoria
        if (imagenAleatoria.ancho != imagenAnterior.ancho || imagenAleatoria.alto != imagenAnterior.alto) {
            throw std::runtime_error("Las dimensiones de la imagen aleatoria no coinciden");
        }
        imagenActual = imagenAnterior.aplicarXOR(imagenAleatoria);
        
        // Guardar imagen intermedia para verificación
        std::string nombreIntermedio = "paso_" + std::to_string(i) + "_reconstruido.bmp";
        guardarImagenBMP(imagenActual, nombreIntermedio);
    }
    
    return imagenActual;
}

// Función para verificar si un directorio existe
bool directorioExiste(const std::string& ruta) {
    return _access(ruta.c_str(), 0) == 0;
}

// Función para obtener la lista de casos disponibles
std::vector<std::string> obtenerCasosDisponibles() {
    std::vector<std::string> casos;
    std::string rutaCasos = "Casos";
    
    // Verificar si existe el directorio
    if (!directorioExiste(rutaCasos)) {
        throw std::runtime_error("No se encontró el directorio 'Casos'");
    }
    
    WIN32_FIND_DATAA findData;
    std::string patronBusqueda = rutaCasos + "\\*";
    HANDLE hFind = FindFirstFileA(patronBusqueda.c_str(), &findData);
    
    if (hFind == INVALID_HANDLE_VALUE) {
        throw std::runtime_error("Error al buscar casos en el directorio");
    }
    
    do {
        if ((findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) &&
            (strcmp(findData.cFileName, ".") != 0) &&
            (strcmp(findData.cFileName, "..") != 0)) {
            casos.push_back(findData.cFileName);
        }
    } while (FindNextFileA(hFind, &findData));
    
    FindClose(hFind);
    
    if (casos.empty()) {
        throw std::runtime_error("No se encontraron casos en el directorio");
    }
    
    // Ordenar los casos
    std::sort(casos.begin(), casos.end());
    return casos;
}

// Función para obtener la lista de archivos M*.txt ordenados
std::vector<std::string> obtenerArchivosMascara(const std::string& rutaBase) {
    std::vector<std::string> archivos;
    std::string patronBusqueda = rutaBase + "M*.txt";
    
    WIN32_FIND_DATAA findData;
    HANDLE hFind = FindFirstFileA(patronBusqueda.c_str(), &findData);
    
    if (hFind == INVALID_HANDLE_VALUE) {
        throw std::runtime_error("No se encontraron archivos de máscara (M*.txt)");
    }
    
    do {
        if (!(findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
            archivos.push_back(findData.cFileName);
        }
    } while (FindNextFileA(hFind, &findData));
    
    FindClose(hFind);
    
    if (archivos.empty()) {
        throw std::runtime_error("No se encontraron archivos de máscara (M*.txt)");
    }
    
    // Ordenar los archivos para procesarlos en orden
    std::sort(archivos.begin(), archivos.end());
    return archivos;
}

int main() {
    try {
        std::cout << "Programa de Reconstrucción de Imágenes" << std::endl;
        std::cout << "======================================" << std::endl;
        
        // Obtener y mostrar casos disponibles
        std::vector<std::string> casos;
        try {
            casos = obtenerCasosDisponibles();
        } catch (const std::runtime_error& e) {
            std::cerr << "Error al buscar casos: " << e.what() << std::endl;
            return 1;
        }
        
        // Mostrar casos disponibles
        std::cout << "\nCasos disponibles:" << std::endl;
        for (size_t i = 0; i < casos.size(); ++i) {
            std::cout << (i + 1) << ". " << casos[i] << std::endl;
        }
        
        // Solicitar selección de caso
        int casoSeleccionado;
        std::cout << "\nSeleccione el caso a procesar (1-" << casos.size() << "): ";
        std::cin >> casoSeleccionado;
        
        // Validar entrada
        if (std::cin.fail() || casoSeleccionado < 1 || casoSeleccionado > static_cast<int>(casos.size())) {
            throw std::runtime_error("Selección de caso no válida");
        }
        
        // Construir ruta base según el caso seleccionado
        std::string rutaBase = "Casos\\" + casos[casoSeleccionado - 1] + "\\";
        
        // Verificar que el directorio del caso existe
        if (!directorioExiste(rutaBase)) {
            throw std::runtime_error("El directorio del caso seleccionado no existe");
        }
        
        std::cout << "\nProcesando " << casos[casoSeleccionado - 1] << "..." << std::endl;
        
        // Rutas de los archivos según el caso seleccionado
        std::string rutaImagenTransformada = rutaBase + "I_M.bmp";
        std::string rutaImagenAleatoria = rutaBase + "I_O.bmp";
        std::string rutaMascara = rutaBase + "M.bmp";
        std::string rutaSalida = rutaBase + "P1.bmp";
        
        // Cargar imágenes
        std::cout << "Cargando imágenes..." << std::endl;
        Imagen imagenTransformada = cargarImagenBMP(rutaImagenTransformada);
        Imagen imagenAleatoria = cargarImagenBMP(rutaImagenAleatoria);
        Imagen mascara = cargarImagenBMP(rutaMascara);
        
        // Validar que las imágenes se cargaron correctamente
        if (imagenTransformada.ancho == 0 || imagenTransformada.alto == 0) {
            throw std::runtime_error("Error al cargar la imagen transformada");
        }
        if (imagenAleatoria.ancho == 0 || imagenAleatoria.alto == 0) {
            throw std::runtime_error("Error al cargar la imagen aleatoria");
        }
        if (mascara.ancho == 0 || mascara.alto == 0) {
            throw std::runtime_error("Error al cargar la máscara");
        }
        
        // Validar dimensiones
        if (imagenTransformada.ancho != imagenAleatoria.ancho || 
            imagenTransformada.alto != imagenAleatoria.alto) {
            throw std::runtime_error("Las dimensiones de las imágenes no coinciden");
        }
        
        // Cargar archivos de enmascaramiento
        std::cout << "Cargando archivos de enmascaramiento..." << std::endl;
        std::vector<DatosMascara> datosMascaras;
        
        // Obtener lista de archivos M*.txt
        std::vector<std::string> archivosMascara = obtenerArchivosMascara(rutaBase);
        
        // Mostrar archivos encontrados
        std::cout << "Archivos de máscara encontrados: " << archivosMascara.size() << std::endl;
        
        // Cargar cada archivo de máscara
        for (const auto& archivo : archivosMascara) {
            std::string rutaArchivo = rutaBase + archivo;
            std::cout << "Cargando: " << archivo << std::endl;
            
            try {
                datosMascaras.push_back(cargarDatosMascara(rutaArchivo));
                std::cout << "Archivo cargado correctamente." << std::endl;
            } catch (const std::exception& e) {
                std::cerr << "Error al cargar " << archivo << ": " << e.what() << std::endl;
                throw;
            }
        }
        
        if (datosMascaras.empty()) {
            throw std::runtime_error("No se pudieron cargar los archivos de enmascaramiento");
        }
        
        // Reconstruir la imagen original
        std::cout << "Reconstruyendo imagen original..." << std::endl;
        Imagen imagenReconstruida = reconstruirImagen(imagenTransformada, imagenAleatoria, mascara, datosMascaras);
        
        // Guardar la imagen reconstruida
        std::cout << "Guardando imagen reconstruida..." << std::endl;
        guardarImagenBMP(imagenReconstruida, rutaSalida);
        
        std::cout << "Proceso completado. La imagen reconstruida se ha guardado en: " << rutaSalida << std::endl;
        
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
}
