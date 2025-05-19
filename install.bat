@echo off
echo Instalando Nam-Pac...
echo.

REM Verifica se está rodando como administrador
net session >nul 2>&1
if %errorLevel% == 0 (
    echo Executando como administrador...
) else (
    echo Por favor, execute este instalador como administrador.
    echo Clique com o botao direito e selecione "Executar como administrador"
    pause
    exit
)

REM Copia o libcurl.dll para System32
echo Copiando bibliotecas necessarias...
copy /Y "libcurl.dll" "C:\Windows\System32\libcurl.dll"

REM Cria um atalho na área de trabalho
echo Criando atalho na area de trabalho...
set SCRIPT="%TEMP%\create-shortcut.vbs"
echo Set oWS = WScript.CreateObject("WScript.Shell") > %SCRIPT%
echo sLinkFile = oWS.SpecialFolders("Desktop") ^& "\Nam-Pac.lnk" >> %SCRIPT%
echo Set oLink = oWS.CreateShortcut(sLinkFile) >> %SCRIPT%
echo oLink.TargetPath = "%~dp0pacman_inverso.exe" >> %SCRIPT%
echo oLink.WorkingDirectory = "%~dp0" >> %SCRIPT%
echo oLink.Save >> %SCRIPT%
cscript /nologo %SCRIPT%
del %SCRIPT%

echo.
echo Instalacao concluida!
echo O jogo foi instalado com sucesso.
echo Voce pode executar o jogo pelo atalho na area de trabalho.
echo.
pause 