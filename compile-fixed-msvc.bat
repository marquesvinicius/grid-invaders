@echo off
echo Compilando Grid Invaders (Versao Corrigida)...
echo.

REM Verifica se o Visual Studio está instalado
where cl >nul 2>&1
if %ERRORLEVEL% NEQ 0 (
    echo Visual Studio nao encontrado!
    echo Por favor, instale o Visual Studio Build Tools.
    echo Download: https://visualstudio.microsoft.com/visual-cpp-build-tools/
    pause
    exit /b 1
)

REM Configura ambiente do Visual Studio
call "C:\Program Files\Microsoft Visual Studio\2022\BuildTools\VC\Auxiliary\Build\vcvars32.bat"

REM Compila usando MSVC (cl.exe)
cl.exe /nologo /W3 /O2 /MD /I"packages\Allegro.5.2.6\build\native\include" ^
    grid-invaders-src\main-fixed.c ^
    /link /SUBSYSTEM:WINDOWS ^
    /LIBPATH:"packages\Allegro.5.2.6\build\native\v142\win32\lib" ^
    allegro-debug-5.2.lib ^
    allegro_main-debug-5.2.lib ^
    allegro_image-debug-5.2.lib ^
    allegro_font-debug-5.2.lib ^
    allegro_ttf-debug-5.2.lib ^
    allegro_primitives-debug-5.2.lib ^
    allegro_audio-debug-5.2.lib ^
    allegro_acodec-debug-5.2.lib ^
    allegro_physfs-debug-5.2.lib ^
    allegro_dialog-debug-5.2.lib ^
    allegro_memfile-debug-5.2.lib ^
    allegro_color-debug-5.2.lib ^
    allegro_video-debug-5.2.lib ^
    /OUT:grid-invaders-fixed.exe

if %ERRORLEVEL% EQU 0 (
    echo.
    echo ========================================
    echo Compilacao concluida com sucesso!
    echo ========================================
    echo.
    echo Para executar o jogo corrigido:
    echo   run-game-complete.bat
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
