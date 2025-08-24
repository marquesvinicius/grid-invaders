@echo off
echo Compilando Grid Invaders (MinGW)...
echo.

REM Verifica se o MinGW está instalado
where gcc >nul 2>&1
if %ERRORLEVEL% NEQ 0 (
    echo GCC/MinGW nao encontrado!
    echo Por favor, instale o MinGW.
    echo Download: https://www.mingw-w64.org/
    pause
    exit /b 1
)

REM Compila usando GCC
gcc -o grid-invaders-mingw.exe ^
    grid-invaders-src/main-fixed.c ^
    -Ipackages/Allegro.5.2.6/build/native/include ^
    -Lpackages/Allegro.5.2.6/build/native/v142/win32/lib ^
    -lallegro-debug-5.2 ^
    -lallegro_main-debug-5.2 ^
    -lallegro_image-debug-5.2 ^
    -lallegro_font-debug-5.2 ^
    -lallegro_ttf-debug-5.2 ^
    -lallegro_primitives-debug-5.2 ^
    -lallegro_audio-debug-5.2 ^
    -lallegro_acodec-debug-5.2 ^
    -lm ^
    -w

if %ERRORLEVEL% EQU 0 (
    echo.
    echo ========================================
    echo Compilacao concluida com sucesso!
    echo ========================================
    echo.
    echo Para executar o jogo:
    echo   grid-invaders-mingw.exe
    echo.
    echo Melhorias implementadas:
    echo   - Menu inicial funcional
    echo   - Audio funcionando (trilha sonora + laser)
    echo   - Spawn controlado de inimigos (2 segundos)
    echo   - Game over quando todos os 18 inimigos aparecem
    echo   - Sistema de pontuacao corrigido
    echo   - Colisoes melhoradas
    echo.
) else (
    echo.
    echo ========================================
    echo Erro na compilacao!
    echo ========================================
    echo.
)

pause
