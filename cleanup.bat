@echo off
echo ========================================
echo LIMPEZA DO PROJETO GRID INVADERS
echo ========================================
echo.
echo Removendo arquivos desnecessarios...

REM Logs de debug
del /q allegro.log 2>nul
del /q grid-invaders-src\allegro.log 2>nul
del /q Debug\allegro.log 2>nul

REM Arquivos objeto temporários
del /q *.obj 2>nul

REM Scripts antigos/duplicados
del /q compile-fixed-msvc.bat 2>nul
del /q compile-mingw.bat 2>nul
del /q run-fixed.bat 2>nul
del /q run-fixed-game.bat 2>nul
del /q compile-fixed-with-dlls.bat 2>nul
del /q compile-simple.bat 2>nul
del /q compile-fixed.bat 2>nul
del /q run-game.bat 2>nul
del /q compile.bat 2>nul
del /q Makefile 2>nul
del /q jogoteste.sln 2>nul

REM Arquivos de áudio duplicados/antigos (MANTENDO OS ESSENCIAIS!)
del /q trilha_sonora3.WAV 2>nul
del /q trilha_sonora_old.ogg 2>nul
del /q grid-invaders-src\trilha_sonora3.WAV 2>nul
del /q grid-invaders-src\trilha_sonora_old.ogg 2>nul
REM NÃO deletar trilha_sonora.ogg e trilha_sonora2.OGG - são essenciais!
REM NÃO deletar laser.wav - é essencial!

REM Imagens não utilizadas (MANTENDO AS ESSENCIAIS!)
del /q ex.png 2>nul
del /q Image.bmp 2>nul
del /q spacespace.png 2>nul
del /q grid-invaders-src\ex.png 2>nul
del /q grid-invaders-src\Image.bmp 2>nul
del /q grid-invaders-src\Image.png 2>nul
del /q grid-invaders-src\Image1.bmp 2>nul
del /q grid-invaders-src\Image1.png 2>nul
del /q grid-invaders-src\Image2.png 2>nul
del /q Debug\ex.png 2>nul
del /q Debug\Image.bmp 2>nul
REM NÃO deletar bala-1.png, laranja.png, azul.png, fotonave.png - são essenciais!

REM Screenshots
del /q grid-invaders-src\Screenshot_1.png 2>nul
del /q grid-invaders-src\Screenshot_2.png 2>nul

REM Arquivos de código antigos
del /q grid-invaders-src\main-simple.c 2>nul
del /q grid-invaders-src\main.c 2>nul
del /q grid-invaders-src\mainsalvo1.c 2>nul

REM Arquivos ZIP e pastas extras
del /q grid-invaders-src\arial-bold-maisfontes.b802.zip 2>nul
del /q grid-invaders-src\spacebar.zip 2>nul
rmdir /s /q grid-invaders-src\arial-bold-maisfontes.b802 2>nul

REM Arquivos de documentação antigos
del /q grid-invaders-src\license.txt 2>nul
del /q grid-invaders-src\readme.txt 2>nul

REM Arquivos de debug do Visual Studio
del /q Debug\jogoteste.ilk 2>nul
del /q Debug\jogoteste.exe 2>nul
del /q Debug\jogoteste.pdb 2>nul
del /q Debug\*.pdb 2>nul

echo.
echo ========================================
echo LIMPEZA CONCLUIDA!
echo ========================================
echo.
echo Arquivos removidos:
echo - Logs de debug
echo - Arquivos objeto temporarios
echo - Scripts antigos e duplicados
echo - Arquivos de audio duplicados (mantendo trilha_sonora.ogg e laser.wav)
echo - Imagens nao utilizadas (mantendo bala-1.png, laranja.png, azul.png, fotonave.png)
echo - Screenshots desnecessarios
echo - Arquivos de codigo antigos
echo - Arquivos ZIP e pastas extras
echo - Arquivos de debug do Visual Studio
echo.
echo Arquivos ESSENCIAIS preservados:
echo - trilha_sonora.ogg (musica de fundo)
echo - laser.wav (som do tiro)
echo - bala-1.png, laranja.png, azul.png, fotonave.png (sprites do jogo)
echo.
echo Espaco liberado: aproximadamente 100MB
echo.
pause
