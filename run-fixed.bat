@echo off
echo ========================================
echo Grid Invaders - Versao Corrigida
echo ========================================
echo.

REM Copia DLLs e assets necessários
echo Copiando arquivos necessarios...
copy Debug\*.dll . >nul 2>&1
copy Debug\*.ttf . >nul 2>&1
copy Debug\*.png . >nul 2>&1
copy Debug\*.bmp . >nul 2>&1
copy Debug\*.ogg . >nul 2>&1
copy Debug\*.wav . >nul 2>&1

REM Copia o executável original
copy Debug\jogoteste.exe grid-invaders-fixed.exe >nul 2>&1

echo.
echo Arquivos copiados com sucesso!
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

echo Iniciando jogo...
echo.
grid-invaders-fixed.exe

echo.
echo Jogo finalizado.
pause
