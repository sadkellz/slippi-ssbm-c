SET "FSPATH=C:\Users\fores\source\repos\slippi-mainline\Binary\x64\Sys\GameFiles\GALE01\"

@REM CSS
"m-ex/MexTK/MexTK.exe" -ff -i "Scenes/CSS/SlippiCSSSetup.c" ^
-s mnFunction ^
-o "%FSPATH%SlippiCSS.dat" ^
-t "m-ex/MexTK/mnFunction.txt" ^
-q -ow -c -l "melee.link"

@REM @REM SSS
@REM "m-ex/MexTK/MexTK.exe" -ff -i "Scenes/3v1/SSS/SSS.c" ^
@REM -s mnFunction ^
@REM -o "%FSPATH%CustomSSS.dat" ^
@REM -t "m-ex/MexTK/mnFunction.txt" ^
@REM -q -ow -c -l "melee.link"

IN-GAME
"m-ex/MexTK/MexTK.exe" -ff -i "Scenes/3v1/VS/Vs.c" ^
-s mnFunction ^
-o "%FSPATH%CustomVs.dat" ^
-t "m-ex/MexTK/mnFunction.txt" ^
-c -q -ow -c -l "melee.link"

@echo off
for /d /r . %%d in (build) do @if exist "%%d" (
    rmdir /s /q "%%d"
)