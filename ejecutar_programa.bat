@echo off
echo Programa de Reconstruccion de Imagenes
echo ====================================
echo.

if "%~1"=="" (
    echo ERROR: Debes especificar el directorio que contiene las imagenes
    echo Ejemplo: ejecutar_programa.bat C:\MisCasos\Caso1
    echo.
    echo El directorio debe contener:
    echo - I_M.bmp
    echo - I_O.bmp
    echo - M.bmp
    echo - Uno o mas archivos M*.txt
    echo - Opcionalmente I_D.bmp
    echo.
    pause
    exit /b 1
)

untitled1.exe "%~1"
echo.
echo Presiona cualquier tecla para cerrar...
pause > nul 