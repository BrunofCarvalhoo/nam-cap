@echo off
echo Iniciando Nam-Pac...
echo.

REM Verifica se o libcurl.dll está na pasta atual
if not exist "libcurl.dll" (
    echo ERRO: Arquivo libcurl.dll nao encontrado!
    echo Por favor, certifique-se de que todos os arquivos estao na mesma pasta.
    pause
    exit
)

REM Verifica se o executável existe
if not exist "pacman_inverso.exe" (
    echo ERRO: Arquivo pacman_inverso.exe nao encontrado!
    echo Por favor, certifique-se de que todos os arquivos estao na mesma pasta.
    pause
    exit
)

REM Tenta copiar o DLL para a pasta atual (caso não esteja lá)
copy /Y "libcurl.dll" ".\libcurl.dll" >nul 2>&1

REM Inicia o jogo
echo Iniciando o jogo...
start "" "pacman_inverso.exe"

REM Se o jogo não iniciar, tenta como administrador
if errorlevel 1 (
    echo Tentando iniciar como administrador...
    powershell -Command "Start-Process 'pacman_inverso.exe' -Verb RunAs"
)

exit 