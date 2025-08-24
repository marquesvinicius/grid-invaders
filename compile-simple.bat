@echo off
echo Compilando Grid Invaders (Versao Simplificada)...
echo.

gcc -o grid-invaders-simple.exe ^
    grid-invaders-src/main-simple.c ^
    -Ipackages/Allegro.5.2.6/build/native/include ^
    -Lpackages/Allegro.5.2.6/build/native/v142/win32/lib ^
    -lallegro-debug ^
    -lallegro_main-debug ^
    -lallegro_image-debug ^
    -lallegro_font-debug ^
    -lallegro_ttf-debug ^
    -lallegro_primitives-debug ^
    -w

if %ERRORLEVEL% EQU 0 (
    echo.
    echo ========================================
    echo Compilacao concluida com sucesso!
    echo ========================================
    echo.
    echo Para executar o jogo:
    echo   grid-invaders-simple.exe
    echo.
    echo Caracteristicas desta versao:
    echo   - Versao simplificada sem audio
    echo   - Graficos basicos (formas geometricas)
    echo   - Menu funcional
    echo   - Game over funcional
    echo   - Spawn controlado de inimigos
    echo   - Sistema de pontuacao
    echo.
) else (
    echo.
    echo ========================================
    echo Erro na compilacao!
    echo ========================================
    echo.
    echo Tentando compilacao alternativa...
    echo.
    
    gcc -o grid-invaders-simple.exe ^
        grid-invaders-src/main-simple.c ^
        -Ipackages/Allegro.5.2.6/build/native/include ^
        -Lpackages/Allegro.5.2.6/build/native/v142/win32/lib ^
        -lallegro-debug ^
        -lallegro_main-debug ^
        -w
        
    if %ERRORLEVEL% EQU 0 (
        echo Compilacao alternativa bem-sucedida!
    ) else (
        echo Falha na compilacao alternativa.
    )
)

pause
