// reconstructor.cpp - Implementación de la clase principal para la reconstrucción de imágenes
#include "reconstructor.h"
#include <QFile>
#include <QTextStream>

Reconstructor::Reconstructor(QObject *parent) : QObject(parent) {
    // Inicializar componentes
    processor = new ImageProcessor(this);
    bmpHandler = new BMPHandler(this);

    // Inicializar punteros a nullptr
    imageID = nullptr;
    imageIM = nullptr;
    maskM = nullptr;
    resultImage = nullptr;
    maskingData = nullptr;
    maskingFilesCount = 0;

    // Inicializar dimensiones
    idWidth = idHeight = 0;
    imWidth = imHeight = 0;
    maskWidth = maskHeight = 0;
}

Reconstructor::~Reconstructor() {
    cleanup();
    delete processor;
    delete bmpHandler;
}

void Reconstructor::cleanup() {
    // Liberar memoria de las imágenes
    delete[] imageID;
    delete[] imageIM;
    delete[] maskM;
    delete[] resultImage;

    // Liberar memoria de los datos de enmascaramiento
    if (maskingData) {
        for (int i = 0; i < maskingFilesCount; i++) {
            delete[] maskingData[i].sums;
        }
        delete[] maskingData;
    }

    // Reiniciar punteros
    imageID = imageIM = maskM = resultImage = nullptr;
    maskingData = nullptr;
    maskingFilesCount = 0;
}

bool Reconstructor::loadImages(const QString& idPath, const QString& imPath, const QString& maskPath) {
    processor->logMessage("Iniciando carga de imágenes");

    // Cargar imagen ID
    if (!bmpHandler->readBMP(idPath, &imageID, idWidth, idHeight)) {
        processor->logMessage("Error al cargar imagen ID", "ERROR");
        return false;
    }

    // Cargar imagen IM
    if (!bmpHandler->readBMP(imPath, &imageIM, imWidth, imHeight)) {
        processor->logMessage("Error al cargar imagen IM", "ERROR");
        return false;
    }

    // Cargar máscara M
    if (!bmpHandler->readBMP(maskPath, &maskM, maskWidth, maskHeight)) {
        processor->logMessage("Error al cargar máscara M", "ERROR");
        return false;
    }

    // Verificar dimensiones
    if (idWidth != imWidth || idHeight != imHeight) {
        processor->logMessage("Las dimensiones de las imágenes no coinciden", "ERROR");
        return false;
    }

    processor->logMessage("Imágenes cargadas exitosamente");
    return true;
}

bool Reconstructor::loadMaskingFiles(const QStringList& maskingFilePaths) {
    maskingFilesCount = maskingFilePaths.size();
    maskingData = new MaskingInfo[maskingFilesCount];

    processor->logMessage(QString("Cargando %1 archivos de enmascaramiento").arg(maskingFilesCount));

    for (int i = 0; i < maskingFilesCount; i++) {
        if (!readMaskingFile(maskingFilePaths[i], i)) {
            processor->logMessage(QString("Error al cargar archivo de enmascaramiento %1").arg(i + 1), "ERROR");
            return false;
        }
    }

    processor->logMessage("Archivos de enmascaramiento cargados exitosamente");
    return true;
}

bool Reconstructor::readMaskingFile(const QString& path, int index) {
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return false;
    }

    QTextStream in(&file);
    
    // Leer desplazamiento
    maskingData[index].displacement = in.readLine().toInt();
    
    // Calcular tamaño de datos
    maskingData[index].size = maskWidth * maskHeight * 3;
    maskingData[index].sums = new int[maskingData[index].size];

    // Leer sumas RGB
    int i = 0;
    while (!in.atEnd() && i < maskingData[index].size) {
        maskingData[index].sums[i++] = in.readLine().toInt();
    }

    file.close();
    return true;
}

bool Reconstructor::reconstruct() {
    processor->logMessage("Iniciando proceso de reconstrucción");

    // Asignar memoria para la imagen resultado
    resultImage = new unsigned char[idWidth * idHeight * 3];

    // Procesar cada paso de la reconstrucción
    for (int step = maskingFilesCount - 1; step >= 0; step--) {
        if (!processStep(step)) {
            processor->logMessage("Error en el proceso de reconstrucción", "ERROR");
            return false;
        }
    }

    processor->logMessage("Reconstrucción completada exitosamente");
    return true;
}

bool Reconstructor::processStep(int step) {
    // Aplicar operaciones inversas según el paso
    if (step == 1) { // Paso 2: Rotación inversa
        processor->rotateRight3Bits(imageID, idWidth * idHeight * 3);
    } else if (step == 0) { // Paso 1: XOR con IM
        processor->xorOperation(imageID, imageIM, resultImage, idWidth * idHeight * 3);
    }

    // Aplicar máscara inversa
    processor->applyMask(resultImage, maskM, maskingData[step].displacement,
                        maskWidth, maskHeight, idWidth);

    return true;
}

bool Reconstructor::saveResult(const QString& outputPath) {
    processor->logMessage("Guardando imagen reconstruida");
    
    if (!bmpHandler->writeBMP(outputPath, resultImage, idWidth, idHeight)) {
        processor->logMessage("Error al guardar la imagen reconstruida", "ERROR");
        return false;
    }

    processor->logMessage("Imagen guardada exitosamente");
    return true;
}