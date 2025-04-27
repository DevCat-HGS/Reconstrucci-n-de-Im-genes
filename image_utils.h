#ifndef IMAGE_UTILS_H
#define IMAGE_UTILS_H

#include <QString>

// Function declarations for image loading and manipulation
unsigned char* loadPixels(QString input, int &width, int &height);
bool exportImage(unsigned char* pixelData, int width, int height, QString archivoSalida);
unsigned int* loadSeedMasking(const char* nombreArchivo, int &seed, int &n_pixels);

#endif // IMAGE_UTILS_H 