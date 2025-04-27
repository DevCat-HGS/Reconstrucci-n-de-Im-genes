// imageprocessor.cpp - Implementación de la clase ImageProcessor
#include "imageprocessor.h"
#include <QDebug>
#include <QImage>

using namespace std;

ImageProcessor::ImageProcessor(QObject *parent) : QObject(parent) {
    initializeLogger();
}

ImageProcessor::~ImageProcessor() {
    if (logStream) {
        delete logStream;
        logStream = nullptr;
    }
    if (logFile) {
        logFile->close();
        delete logFile;
        logFile = nullptr;
    }
}

void ImageProcessor::initializeLogger() {
    // Crear archivo de log con timestamp
    QString timestamp = QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss");
    logFile = new QFile(QString("reconstruction_%1.log").arg(timestamp));
    
    if (logFile->open(QIODevice::WriteOnly | QIODevice::Text)) {
        logStream = new QTextStream(logFile);
        logMessage("Sistema de reconstrucción de imágenes iniciado");
    }
}

void ImageProcessor::logMessage(const QString& message, const QString& level) {
    if (logStream) {
        *logStream << QString("%1 [%2] %3\n")
                      .arg(getCurrentTimestamp())
                      .arg(level)
                      .arg(message);
        logStream->flush();
    }
}

QString ImageProcessor::getCurrentTimestamp() const {
    return QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz");
}

bool ImageProcessor::loadImage(const QString& path, unsigned char** data, int& width, int& height) {
    QImage image(path);
    if (image.isNull()) {
        logMessage(QString("Error al cargar la imagen: %1").arg(path), "ERROR");
        return false;
    }

    width = image.width();
    height = image.height();
    int size = width * height * 3; // RGB format

    *data = new unsigned char[size];
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            QRgb pixel = image.pixel(x, y);
            int pos = (y * width + x) * 3;
            (*data)[pos] = qRed(pixel);
            (*data)[pos + 1] = qGreen(pixel);
            (*data)[pos + 2] = qBlue(pixel);
        }
    }

    logMessage(QString("Imagen cargada exitosamente: %1 (%2x%3)").arg(path).arg(width).arg(height));
    return true;
}

bool ImageProcessor::saveImage(const QString& path, unsigned char* data, int width, int height) {
    if (!data || width <= 0 || height <= 0) {
        logMessage("Datos de imagen inválidos", "ERROR");
        return false;
    }

    QImage image(width, height, QImage::Format_RGB888);
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int pos = (y * width + x) * 3;
            image.setPixel(x, y, qRgb(data[pos], data[pos + 1], data[pos + 2]));
        }
    }

    if (!image.save(path)) {
        logMessage(QString("Error al guardar la imagen: %1").arg(path), "ERROR");
        return false;
    }

    logMessage(QString("Imagen guardada exitosamente: %1").arg(path));
    return true;
}

void ImageProcessor::xorOperation(unsigned char* img1, unsigned char* img2, unsigned char* result, int size) {
    logMessage("Iniciando operación XOR");
    for (int i = 0; i < size; i++) {
        result[i] = img1[i] ^ img2[i];
    }
    logMessage("Operación XOR completada");
}

void ImageProcessor::rotateRight3Bits(unsigned char* data, int size) {
    logMessage("Iniciando rotación de bits");
    for (int i = 0; i < size; i++) {
        unsigned char value = data[i];
        data[i] = (value >> 3) | (value << 5);
    }
    logMessage("Rotación de bits completada");
}

void ImageProcessor::applyMask(unsigned char* image, unsigned char* mask, 
                              int displacement, int maskWidth, int maskHeight, 
                              int imageWidth) {
    if (!image || !mask || maskWidth <= 0 || maskHeight <= 0 || imageWidth <= 0) {
        logMessage("Parámetros inválidos en applyMask", "ERROR");
        return;
    }

    logMessage(QString("Aplicando máscara con desplazamiento: %1").arg(displacement));
    
    // Aplicar la máscara a la imagen
    for (int y = 0; y < maskHeight; y++) {
        for (int x = 0; x < maskWidth; x++) {
            int imgPos = (y * imageWidth + x + displacement) * 3;
            int maskPos = (y * maskWidth + x) * 3;
            
            // Aplicar máscara a cada canal de color
            image[imgPos] &= mask[maskPos];       // Canal Rojo
            image[imgPos + 1] &= mask[maskPos + 1]; // Canal Verde
            image[imgPos + 2] &= mask[maskPos + 2]; // Canal Azul
        }
    }
    
    logMessage("Aplicación de máscara completada");
}