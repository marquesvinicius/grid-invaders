@echo off
echo ========================================
echo Grid Invaders - Versao Corrigida
echo ========================================
echo.

REM Verificar se existe o executável corrigido
if exist "grid-invaders-fixed.exe" (
    echo Executando versao corrigida...
    echo.
    echo Melhorias implementadas:
    echo   - Menu inicial funcional
    echo   - Audio funcionando (trilha sonora + laser)
    echo   - Spawn controlado de inimigos (2 segundos)
    echo   - Game over quando todos os 18 inimigos aparecem
    echo   - Sistema de pontuacao corrigido
    echo   - Colisoes melhoradas
    echo.
    echo Controles:
    echo   ENTER: Iniciar jogo / Voltar ao menu
    echo   Setas: Mover nave
    echo   ESPACO: Atirar
    echo   ESC: Sair
    echo.
    pause
    grid-invaders-fixed.exe
) else (
    echo Executavel corrigido nao encontrado.
    echo Executando versao original...
    echo.
    echo NOTA: Esta versao pode ter bugs conhecidos:
    echo   - Sem audio
    echo   - Spawn muito rapido de inimigos
    echo   - Game over nao funciona corretamente
    echo.
    pause
    .\run-game.bat
)

echo.
echo Jogo finalizado.
pause
