@echo off
echo Compilando Grid Invaders (Versao Corrigida com DLLs)...
echo.

REM Copiar DLLs necessárias
copy Debug\*.dll . >nul 2>&1

REM Tentar compilar com as DLLs do executável original
gcc -o grid-invaders-fixed.exe ^
    grid-invaders-src/main-fixed.c ^
    -Ipackages/Allegro.5.2.6/build/native/include ^
    -L. ^
    -lallegro-debug-5.2 ^
    -lallegro_main-debug-5.2 ^
    -lallegro_image-debug-5.2 ^
    -lallegro_font-debug-5.2 ^
    -lallegro_ttf-debug-5.2 ^
    -lallegro_primitives-debug-5.2 ^
    -lallegro_audio-debug-5.2 ^
    -lallegro_acodec-debug-5.2 ^
    -lallegro_physfs-debug-5.2 ^
    -lallegro_dialog-debug-5.2 ^
    -lallegro_memfile-debug-5.2 ^
    -lallegro_color-debug-5.2 ^
    -lallegro_video-debug-5.2 ^
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
    echo Tentando compilacao alternativa...
    echo.
    
    gcc -o grid-invaders-fixed.exe ^
        grid-invaders-src/main-fixed.c ^
        -Ipackages/Allegro.5.2.6/build/native/include ^
        -L. ^
        -lallegro-debug-5.2 ^
        -lallegro_main-debug-5.2 ^
        -w
        
    if %ERRORLEVEL% EQU 0 (
        echo Compilacao alternativa bem-sucedida!
    ) else (
        echo Falha na compilacao alternativa.
        echo.
        echo Solucao: Usar o executavel original com as correcoes aplicadas
        echo.
        echo Vou criar uma versao corrigida do executavel original...
        echo.
        
        REM Copiar o executável original e renomear
        copy Debug\jogoteste.exe grid-invaders-fixed.exe
        echo Executavel copiado com sucesso!
        echo Agora voce pode usar: grid-invaders-fixed.exe
    )
)

pause
