@echo off

REM Verifica si se proporcionó un directorio como argumento
if "%1"=="" (
    echo Usage: %0 ^<directory^>
    exit /b 1
)

REM Elimina los archivos con extensión .import en el directorio especificado
for /r "%1" %%f in (*.import) do (
    echo Eliminando %%f
    del "%%f"
)