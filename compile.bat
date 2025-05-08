@echo off
gcc pacman_inverso.c -o pacman_jogo.exe -lgdi32 -luser32 -lkernel32
if %errorlevel% equ 0 (
    echo Compilacao concluida com sucesso!
    echo Execute o jogo com: pacman_jogo.exe
) else (
    echo Erro na compilacao!
)
pause 