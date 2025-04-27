

#include <QCoreApplication>
#include <QCommandLineParser>
#include <QCommandLineOption>
#include <QDebug>
#include <QDir>
#include <QFileInfo>
#include "reconstructor.h"

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);
    QCoreApplication::setApplicationName("ImageReconstructor");
    QCoreApplication::setApplicationVersion("1.0");

    // Configurar el parser de línea de comandos
    QCommandLineParser parser;
    parser.setApplicationDescription("Programa de reconstrucción de imágenes");
    parser.addHelpOption();
    parser.addVersionOption();

    // Definir opciones
    QCommandLineOption idOption(QStringList() << "i" << "id",
        "Imagen ID transformada", "archivo");
    QCommandLineOption imOption(QStringList() << "m" << "im",
        "Imagen IM aleatoria", "archivo");
    QCommandLineOption maskOption(QStringList() << "k" << "mask",
        "Máscara M", "archivo");
    QCommandLineOption outputOption(QStringList() << "o" << "output",
        "Archivo de salida", "archivo");
    QCommandLineOption maskingFilesOption(QStringList() << "f" << "files",
        "Archivos de enmascaramiento (separados por comas)", "archivos");

    // Agregar opciones al parser
    parser.addOption(idOption);
    parser.addOption(imOption);
    parser.addOption(maskOption);
    parser.addOption(outputOption);
    parser.addOption(maskingFilesOption);

    // Procesar los argumentos
    parser.process(a);

    // Verificar opciones obligatorias
    if (!parser.isSet(idOption) || !parser.isSet(imOption) ||
        !parser.isSet(maskOption) || !parser.isSet(outputOption) ||
        !parser.isSet(maskingFilesOption)) {
        parser.showHelp(1);
    }

    // Crear instancia del reconstructor
    Reconstructor reconstructor;

    // Obtener el directorio actual del proyecto
    QDir projectDir = QDir::current();

    // Verificar la existencia de los archivos de entrada
    QFileInfo idFile(projectDir.filePath(parser.value(idOption)));
    QFileInfo imFile(projectDir.filePath(parser.value(imOption)));
    QFileInfo maskFile(projectDir.filePath(parser.value(maskOption)));

    if (!idFile.exists() || !imFile.exists() || !maskFile.exists()) {
        qDebug() << "Error: No se encontraron uno o más archivos de entrada en el directorio del proyecto";
        return 1;
    }

    // Cargar imágenes usando rutas relativas al directorio del proyecto
    if (!reconstructor.loadImages(
            idFile.filePath(),
            imFile.filePath(),
            maskFile.filePath())) {
        return 1;
    }

    // Procesar y verificar archivos de enmascaramiento
    QStringList maskingFilesInput = parser.value(maskingFilesOption).split(',');
    QStringList maskingFiles;

    // Verificar cada archivo de enmascaramiento
    for (const QString& maskFile : maskingFilesInput) {
        QFileInfo maskingFileInfo(projectDir.filePath(maskFile.trimmed()));
        if (!maskingFileInfo.exists()) {
            qDebug() << "Error: No se encontró el archivo de enmascaramiento:" << maskFile;
            return 1;
        }
        maskingFiles.append(maskingFileInfo.filePath());
    }

    // Cargar archivos de enmascaramiento verificados
    if (!reconstructor.loadMaskingFiles(maskingFiles)) {
        return 1;
    }

    // Realizar la reconstrucción
    if (!reconstructor.reconstruct()) {
        return 1;
    }

    // Guardar resultado
    if (!reconstructor.saveResult(parser.value(outputOption))) {
        return 1;
    }

    return 0;
}
 
 
 
 
 
 
 
 
 
 
 
 
 
