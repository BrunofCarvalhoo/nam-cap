@echo off
gcc pacman_inverso.c -o pacman_inverso.exe -lcurl -lgdi32 -luser32 -lkernel32
if %errorlevel% equ 0 (
    echo Compilacao concluida com sucesso!
    echo Execute o jogo com: pacman_inverso.exe
) else (
    echo Erro na compilacao!
)
pause 