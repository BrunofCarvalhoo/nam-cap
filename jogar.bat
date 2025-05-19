@echo off
echo Iniciando Nam-Pac...
echo.

REM Verifica se o GCC está instalado
where gcc >nul 2>&1
if errorlevel 1 (
    echo ERRO: GCC (MinGW) nao encontrado no PATH do sistema!
    echo Instale o MinGW e adicione ao PATH antes de continuar.
    pause
    exit
)

REM Verifica se o libcurl-4.dll existe ou se precisa ser renomeado
set "DLL_FOUND=0"
if exist "libcurl-4.dll" (
    set "DLL_NAME=libcurl-4.dll"
    set "DLL_FOUND=1"
) else if exist "libcurl-x64.dll" (
    copy /Y "libcurl-x64.dll" "libcurl-4.dll" >nul 2>&1
    set "DLL_NAME=libcurl-4.dll"
    set "DLL_FOUND=1"
) else if exist "libcurl.dll" (
    copy /Y "libcurl.dll" "libcurl-4.dll" >nul 2>&1
    set "DLL_NAME=libcurl-4.dll"
    set "DLL_FOUND=1"
)

if %DLL_FOUND%==0 (
    echo ERRO: Nenhum arquivo libcurl-4.dll, libcurl-x64.dll ou libcurl.dll encontrado!
    echo Por favor, coloque o arquivo DLL correto na pasta do jogo.
    pause
    exit
)

REM Compila o jogo se o executável não existir
if not exist "pacman_inverso.exe" (
    echo Compilando o jogo...
    gcc -o pacman_inverso.exe pacman_inverso.c -lgdi32 -lcurl
    if errorlevel 1 (
        echo ERRO ao compilar o jogo!
        echo Verifique se o GCC e a libcurl estao corretamente instalados.
        pause
        exit
    )
    echo Compilacao concluida!
)

REM Inicia o jogo
echo Iniciando o jogo...
start "" "pacman_inverso.exe"

REM Se o jogo não iniciar, tenta como administrador
if errorlevel 1 (
    echo Tentando iniciar como administrador...
    powershell -Command "Start-Process 'pacman_inverso.exe' -Verb RunAs"
)

exit