@echo off
setlocal

:: Stelle sicher, dass ein gültiger Parameter übergeben wurde
if "%1"=="" (
    echo Bitte gib "release" oder "debug" als Parameter an.
    exit /b 1
)

set CONFIG=%1

:: Konvertiere in Großbuchstaben für CMake
if /I "%CONFIG%"=="release" (
    set BUILD_TYPE=Release
) else if /I "%CONFIG%"=="debug" (
    set BUILD_TYPE=Debug
) else (
    echo Ungueltiger Build-Typ: %CONFIG%
    exit /b 1
)

:: Erstelle build-Ordner falls nicht vorhanden
if not exist build (
    mkdir build
)

TITLE 079 System
CLS
REM Compile 079/TheBear & 079/GlitchBear
cd ./build
START /wait cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=%BUILD_TYPE% ..
START /wait make
if /I %ErrorLevel%==0 (
START /wait ./TBaGB079_DEBUG.exe -TB -E Intern 725 -out default
echo %ErrorLevel%
)
cd ./..
PAUSE