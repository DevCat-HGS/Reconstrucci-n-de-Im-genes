 // imageprocessor.h - Clase para el procesamiento de imágenes y reconstrucción
#ifndef IMAGEPROCESSOR_H
#define IMAGEPROCESSOR_H

#include <QObject>
#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include <QImage>
#include <QRgb>

class ImageProcessor : public QObject {
    Q_OBJECT

public:
    explicit ImageProcessor(QObject *parent = nullptr);
    ~ImageProcessor();

    // Métodos para el procesamiento de imágenes
    bool loadImage(const QString& path, unsigned char** data, int& width, int& height);
    bool saveImage(const QString& path, unsigned char* data, int width, int height);
    void xorOperation(unsigned char* img1, unsigned char* img2, unsigned char* result, int size);
    void rotateRight3Bits(unsigned char* data, int size);
    void applyMask(unsigned char* image, unsigned char* mask, int displacement, int maskWidth, int maskHeight, int imageWidth);

    // Sistema de logging
    void logMessage(const QString& message, const QString& level = "INFO");

private:
    // Archivo de log
    QFile* logFile;
    QTextStream* logStream;

    // Métodos auxiliares
    void initializeLogger();
    QString getCurrentTimestamp() const;
};

#endif // IMAGEPROCESSOR_H