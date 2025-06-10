@echo off
if not exist "output" mkdir "output"

@REM Major
"m-ex\MexTK\MexTK.exe" -ff -i "Scenes\SlippiMajor\Major.c" ^
-s mjFunction ^
-o "output\SlippiMajor.dat" ^
-t "m-ex\MexTK\mjFunction.txt" ^
-q -ow -c -l "melee.link"

@REM @REM Ranked
@REM "m-ex\MexTK\MexTK.exe" -ff -i "Scenes\Ranked\GameSetup.c" "Components\CharStageBoxSelector.c" "Components\CharStageIcon.c" "Components\Button.c" "Components\FlatTexture.c" "Components\RightArrow.c" "Components\CharPickerDialog.c" "Components\StockIcon.c" "Components\GameResult.c" "Components\TurnIndicator.c" "Game\Characters.c" ^
@REM -s mnFunction ^
@REM -o "output\GameSetup.dat" ^
@REM -t "m-ex\MexTK\mnFunction.txt" ^
@REM -q -ow -c -l "melee.link"

@REM @REM CSS
@REM "m-ex\MexTK\MexTK.exe" -ff -i "Scenes\CSS\SlippiCSSSetup.c" ^
@REM -s mnFunction ^
@REM -o "output\SlippiCSS.dat" ^
@REM -t "m-ex\MexTK\mnFunction.txt" ^
@REM -q -ow -c -l "melee.link"

move "output\*.dat" "C:\Users\fores\source\repos\slippi-mainline\Binary\x64\Sys\GameFiles\GALE01\"
