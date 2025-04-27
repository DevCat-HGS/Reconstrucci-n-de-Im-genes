// bmphandler.cpp - Implementación de la clase BMPHandler
#include "bmphandler.h"
#include <QFile>
#include <QDataStream>

BMPHandler::BMPHandler(QObject *parent) : QObject(parent) {}

BMPHandler::~BMPHandler() {}

void BMPHandler::handleFileError(const QString& error) {
    qDebug() << "Error de archivo BMP:" << error;
}

bool BMPHandler::readBMP(const QString& filename, unsigned char** imageData, int& width, int& height) {
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly)) {
        return false;
    }

    // Leer encabezados
    BMPHeader header;
    BMPInfoHeader infoHeader;
    if (!readHeaders(file, header, infoHeader)) {
        file.close();
        return false;
    }

    // Verificar formato (24 bits por píxel)
    if (infoHeader.bitsPerPixel != 24) {
        file.close();
        return false;
    }

    // Asignar dimensiones
    width = infoHeader.width;
    height = infoHeader.height;

    // Calcular padding
    int padding = calculatePadding(width);

    // Asignar memoria para los datos de la imagen
    int dataSize = height * width * 3; // 3 bytes por píxel (RGB)
    *imageData = new unsigned char[dataSize];

    // Posicionar en el inicio de los datos
    file.seek(header.dataOffset);

    // Leer datos de la imagen
    for (int y = height - 1; y >= 0; y--) { // BMP almacena las filas de abajo hacia arriba
        // Leer una fila de píxeles
        file.read(reinterpret_cast<char*>(*imageData + y * width * 3), width * 3);
        
        // Saltar el padding
        if (padding > 0) {
            file.seek(file.pos() + padding);
        }
    }

    file.close();
    return true;
}

bool BMPHandler::writeBMP(const QString& filename, unsigned char* imageData, int width, int height) {
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly)) {
        return false;
    }

    // Escribir encabezados
    if (!writeHeaders(file, width, height)) {
        file.close();
        return false;
    }

    // Calcular padding
    int padding = calculatePadding(width);
    unsigned char paddingData[3] = {0, 0, 0};

    // Escribir datos de la imagen
    for (int y = height - 1; y >= 0; y--) { // Escribir de abajo hacia arriba
        // Escribir una fila de píxeles
        file.write(reinterpret_cast<char*>(imageData + y * width * 3), width * 3);
        
        // Escribir padding si es necesario
        if (padding > 0) {
            file.write(reinterpret_cast<char*>(paddingData), padding);
        }
    }

    file.close();
    return true;
}

bool BMPHandler::readHeaders(QFile& file, BMPHeader& header, BMPInfoHeader& infoHeader) {
    QDataStream stream(&file);
    stream.setByteOrder(QDataStream::LittleEndian);

    // Leer encabezado BMP
    stream >> header.signature;
    if (header.signature != 0x4D42) { // 'BM' en little endian
        return false;
    }
    stream >> header.fileSize;
    stream >> header.reserved1;
    stream >> header.reserved2;
    stream >> header.dataOffset;

    // Leer información del encabezado
    stream >> infoHeader.size;
    stream >> infoHeader.width;
    stream >> infoHeader.height;
    stream >> infoHeader.planes;
    stream >> infoHeader.bitsPerPixel;
    stream >> infoHeader.compression;
    stream >> infoHeader.imageSize;
    stream >> infoHeader.xPixelsPerMeter;
    stream >> infoHeader.yPixelsPerMeter;
    stream >> infoHeader.colorsUsed;
    stream >> infoHeader.importantColors;

    return true;
}

bool BMPHandler::writeHeaders(QFile& file, int width, int height) {
    QDataStream stream(&file);
    stream.setByteOrder(QDataStream::LittleEndian);

    // Calcular tamaños
    int padding = calculatePadding(width);
    int dataSize = height * (width * 3 + padding);
    int fileSize = 54 + dataSize; // 54 bytes de encabezados + datos

    // Escribir encabezado BMP
    stream << (unsigned short)0x4D42; // 'BM'
    stream << (unsigned int)fileSize;
    stream << (unsigned short)0; // reserved1
    stream << (unsigned short)0; // reserved2
    stream << (unsigned int)54; // dataOffset

    // Escribir información del encabezado
    stream << (unsigned int)40; // size of info header
    stream << (int)width;
    stream << (int)height;
    stream << (unsigned short)1; // planes
    stream << (unsigned short)24; // bits per pixel
    stream << (unsigned int)0; // compression
    stream << (unsigned int)dataSize;
    stream << (int)2835; // xPixelsPerMeter (72 DPI)
    stream << (int)2835; // yPixelsPerMeter (72 DPI)
    stream << (unsigned int)0; // colorsUsed
    stream << (unsigned int)0; // importantColors

    return true;
}

int BMPHandler::calculatePadding(int width) const {
    return (4 - (width * 3) % 4) % 4; // Cada fila debe ser múltiplo de 4 bytes
}

// Implementación de operaciones a nivel de bits
void BMPHandler::applyXOR(unsigned char* imageData, int dataSize, unsigned char mask) {
    for (int i = 0; i < dataSize; i++) {
        imageData[i] ^= mask;
    }
}

void BMPHandler::applyBitShift(unsigned char* imageData, int dataSize, int shiftAmount, bool isLeftShift) {
    shiftAmount = shiftAmount % 8; // Limitar a 8 bits
    if (shiftAmount == 0) return;

    for (int i = 0; i < dataSize; i++) {
        if (isLeftShift) {
            imageData[i] = (imageData[i] << shiftAmount) | (imageData[i] >> (8 - shiftAmount));
        } else {
            imageData[i] = (imageData[i] >> shiftAmount) | (imageData[i] << (8 - shiftAmount));
        }
    }
}

void BMPHandler::applyBitRotation(unsigned char* imageData, int dataSize, int rotateAmount, bool isLeftRotate) {
    rotateAmount = rotateAmount % 8; // Limitar a 8 bits
    if (rotateAmount == 0) return;

    for (int i = 0; i < dataSize; i++) {
        if (isLeftRotate) {
            imageData[i] = (imageData[i] << rotateAmount) | (imageData[i] >> (8 - rotateAmount));
        } else {
            imageData[i] = (imageData[i] >> rotateAmount) | (imageData[i] << (8 - rotateAmount));
        }
    }
}

// Implementación de verificación y análisis
bool BMPHandler::verifyTransformation(const unsigned char* originalData, const unsigned char* transformedData,
                                     unsigned char mask, int dataSize) {
    for (int i = 0; i < dataSize; i++) {
        if ((originalData[i] ^ mask) != transformedData[i]) {
            return false;
        }
    }
    return true;
}

bool BMPHandler::detectTransformations(const unsigned char* originalData, const unsigned char* transformedData,
                                      int dataSize, QList<QString>& appliedOperations) {
    appliedOperations.clear();
    
    // Detectar XOR
    unsigned char possibleMask = originalData[0] ^ transformedData[0];
    bool isXOR = true;
    for (int i = 1; i < dataSize && isXOR; i++) {
        if ((originalData[i] ^ possibleMask) != transformedData[i]) {
            isXOR = false;
        }
    }
    if (isXOR) {
        appliedOperations.append(QString("XOR con máscara: %1").arg(possibleMask));
        return true;
    }

    // Detectar rotación/desplazamiento
    for (int bits = 1; bits <= 7; bits++) {
        bool isLeftShift = true;
        bool isRightShift = true;
        bool isLeftRotate = true;
        bool isRightRotate = true;

        for (int i = 0; i < dataSize && (isLeftShift || isRightShift || isLeftRotate || isRightRotate); i++) {
            unsigned char leftShifted = (originalData[i] << bits) | (originalData[i] >> (8 - bits));
            unsigned char rightShifted = (originalData[i] >> bits) | (originalData[i] << (8 - bits));
            
            if (leftShifted != transformedData[i]) isLeftShift = false;
            if (rightShifted != transformedData[i]) isRightShift = false;
            if (leftShifted != transformedData[i]) isLeftRotate = false;
            if (rightShifted != transformedData[i]) isRightRotate = false;
        }

        if (isLeftShift) {
            appliedOperations.append(QString("Desplazamiento izquierdo de %1 bits").arg(bits));
            return true;
        } else if (isRightShift) {
            appliedOperations.append(QString("Desplazamiento derecho de %1 bits").arg(bits));
            return true;
        } else if (isLeftRotate) {
            appliedOperations.append(QString("Rotación izquierda de %1 bits").arg(bits));
            return true;
        } else if (isRightRotate) {
            appliedOperations.append(QString("Rotación derecha de %1 bits").arg(bits));
            return true;
        }
    }

    return false;
}
