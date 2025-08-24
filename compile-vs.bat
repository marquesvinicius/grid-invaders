@echo off
echo Compilando Grid Invaders (Visual Studio Build Tools)...
echo.

REM Tenta diferentes caminhos do Visual Studio
set VS_PATH=""

if exist "C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\VC\Auxiliary\Build\vcvars32.bat" (
    set "VS_PATH=C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\VC\Auxiliary\Build\vcvars32.bat"
) else if exist "C:\Program Files\Microsoft Visual Studio\2022\BuildTools\VC\Auxiliary\Build\vcvars32.bat" (
    set "VS_PATH=C:\Program Files\Microsoft Visual Studio\2022\BuildTools\VC\Auxiliary\Build\vcvars32.bat"
) else if exist "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars32.bat" (
    set "VS_PATH=C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars32.bat"
) else if exist "C:\Program Files\Microsoft Visual Studio\2022\Professional\VC\Auxiliary\Build\vcvars32.bat" (
    set "VS_PATH=C:\Program Files\Microsoft Visual Studio\2022\Professional\VC\Auxiliary\Build\vcvars32.bat"
) else if exist "C:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Auxiliary\Build\vcvars32.bat" (
    set "VS_PATH=C:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Auxiliary\Build\vcvars32.bat"
)

if "%VS_PATH%"=="" (
    echo Visual Studio nao encontrado!
    echo Por favor, instale o Visual Studio Build Tools.
    echo Download: https://visualstudio.microsoft.com/visual-cpp-build-tools/
    pause
    exit /b 1
)

echo Configurando ambiente do Visual Studio...
call "%VS_PATH%"

echo Compilando...
cl.exe /nologo /W3 /O2 /MD /I"packages\Allegro.5.2.6\build\native\include" ^
    grid-invaders-src\main-fixed.c ^
    /link /SUBSYSTEM:WINDOWS ^
    /LIBPATH:"packages\Allegro.5.2.6\build\native\v142\win32\lib" ^
    allegro-debug.lib ^
    allegro_main-debug.lib ^
    allegro_image-debug.lib ^
    allegro_font-debug.lib ^
    allegro_ttf-debug.lib ^
    allegro_primitives-debug.lib ^
    allegro_audio-debug.lib ^
    allegro_acodec-debug.lib ^
    allegro_physfs-debug.lib ^
    allegro_dialog-debug.lib ^
    allegro_memfile-debug.lib ^
    allegro_color-debug.lib ^
    allegro_video-debug.lib ^
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
