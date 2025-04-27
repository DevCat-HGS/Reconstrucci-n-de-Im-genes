// reconstructor.h - Clase principal para la reconstrucción de imágenes
#ifndef RECONSTRUCTOR_H
#define RECONSTRUCTOR_H

#include "imageprocessor.h"
#include "bmphandler.h"
#include <QObject>
#include <QString>

class Reconstructor : public QObject {
    Q_OBJECT

public:
    explicit Reconstructor(QObject *parent = nullptr);
    ~Reconstructor();

    // Métodos principales para la reconstrucción
    bool loadImages(const QString& idPath, const QString& imPath, const QString& maskPath);
    bool loadMaskingFiles(const QStringList& maskingFilePaths);
    bool reconstruct();
    bool saveResult(const QString& outputPath);

private:
    // Componentes del sistema
    ImageProcessor* processor;
    BMPHandler* bmpHandler;

    // Datos de las imágenes
    unsigned char* imageID;    // Imagen transformada final
    unsigned char* imageIM;    // Imagen aleatoria
    unsigned char* maskM;      // Máscara
    unsigned char* resultImage; // Imagen reconstruida

    // Dimensiones
    int idWidth, idHeight;
    int imWidth, imHeight;
    int maskWidth, maskHeight;

    // Información de enmascaramiento
    struct MaskingInfo {
        int displacement;
        int* sums;
        int size;
    };
    MaskingInfo* maskingData;
    int maskingFilesCount;

    // Métodos auxiliares
    void cleanup();
    bool processStep(int step);
    bool readMaskingFile(const QString& path, int index);
};

#endif // RECONSTRUCTOR_H