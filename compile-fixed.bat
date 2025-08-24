@echo off
echo Compilando Grid Invaders (Versao Corrigida)...
echo.

gcc -o grid-invaders-fixed.exe ^
    grid-invaders-src/main-fixed.c ^
    -Ipackages/Allegro.5.2.6/build/native/include ^
    -Lpackages/Allegro.5.2.6/build/native/v142/win32/lib ^
    -lallegro-debug ^
    -lallegro_main-debug ^
    -lallegro_image-debug ^
    -lallegro_font-debug ^
    -lallegro_ttf-debug ^
    -lallegro_primitives-debug ^
    -lallegro_audio-debug ^
    -lallegro_acodec-debug ^
    -lallegro_physfs-debug ^
    -lallegro_dialog-debug ^
    -lallegro_memfile-debug ^
    -lallegro_color-debug ^
    -lallegro_video-debug ^
    -w

if %ERRORLEVEL% EQU 0 (
    echo.
    echo ========================================
    echo Compilacao concluida com sucesso!
    echo ========================================
    echo.
    echo Para executar o jogo corrigido:
    echo   grid-invaders-fixed.exe
    echo.
    echo Melhorias implementadas:
    echo   - Menu inicial
    echo   - Audio funcionando
    echo   - Spawn de inimigos controlado
    echo   - Game over funcional
    echo   - Sistema de pontuacao corrigido
    echo.
) else (
    echo.
    echo ========================================
    echo Erro na compilacao!
    echo ========================================
    echo.
)

pause
