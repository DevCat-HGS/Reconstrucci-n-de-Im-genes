// bmphandler.h - Clase para manejo de archivos BMP
#ifndef BMPHANDLER_H
#define BMPHANDLER_H

#include <QObject>
#include <QFile>
#include <QDebug>

// Estructura para el encabezado del archivo BMP
struct BMPHeader {
    unsigned short signature;      // Debe ser 'BM'
    unsigned int fileSize;        // Tamaño total del archivo
    unsigned short reserved1;     // Reservado
    unsigned short reserved2;     // Reservado
    unsigned int dataOffset;      // Offset donde comienzan los datos de la imagen
};

// Estructura para la información del encabezado BMP
struct BMPInfoHeader {
    unsigned int size;            // Tamaño de este encabezado
    int width;                    // Ancho de la imagen
    int height;                   // Alto de la imagen
    unsigned short planes;        // Número de planos (debe ser 1)
    unsigned short bitsPerPixel;  // Bits por píxel
    unsigned int compression;     // Tipo de compresión
    unsigned int imageSize;       // Tamaño de la imagen
    int xPixelsPerMeter;         // Resolución horizontal
    int yPixelsPerMeter;         // Resolución vertical
    unsigned int colorsUsed;      // Número de colores en la paleta
    unsigned int importantColors; // Número de colores importantes
};

class BMPHandler : public QObject {
    Q_OBJECT

public:
    explicit BMPHandler(QObject *parent = nullptr);
    ~BMPHandler();

    // Métodos principales
    bool readBMP(const QString& filename, unsigned char** imageData, int& width, int& height);
    bool writeBMP(const QString& filename, unsigned char* imageData, int width, int height);

    // Operaciones a nivel de bits
    void applyXOR(unsigned char* imageData, int dataSize, unsigned char mask);
    void applyBitShift(unsigned char* imageData, int dataSize, int shiftAmount, bool isLeftShift);
    void applyBitRotation(unsigned char* imageData, int dataSize, int rotateAmount, bool isLeftRotate);
    
    // Verificación y análisis
    bool verifyTransformation(const unsigned char* originalData, const unsigned char* transformedData, 
                            unsigned char mask, int dataSize);
    bool detectTransformations(const unsigned char* originalData, const unsigned char* transformedData, 
                             int dataSize, QList<QString>& appliedOperations);

private:
    // Métodos auxiliares
    bool readHeaders(QFile& file, BMPHeader& header, BMPInfoHeader& infoHeader);
    bool writeHeaders(QFile& file, int width, int height);
    int calculatePadding(int width) const;

private slots:
    void handleFileError(const QString& error);
};

#endif // BMPHANDLER_H