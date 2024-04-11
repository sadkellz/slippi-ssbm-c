@REM SET "TKPATH=../../MexTK/"

:: compile main code
@REM "%TKPATH%MexTK.exe" -ff -i "src/MWireFrame.c" -s ftFunction -dat "PlBo.dat" -ow 
"m-ex/MexTK/MexTK.exe" -ff -i "MWireFrame/src/MWireFrame.c" -s ftFunction -dat "MWireFrame/PlBo.dat" -ow 

:: compile item code
@REM "%TKPATH%MexTK.exe" -ff -item 0 "src/item_fireball.c" -item 2 "src/item_cape.c" -dat "PlMr.dat" -s itFunction -ow
