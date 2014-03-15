@echo off
del find03.exe
e:\Borland\bcc55\Bin\bcc32.exe -D_MSDOS_ -Ie:\Borland\bcc55\Include -c FIND03.C hash.c ../conv.c
e:\Borland\bcc55\Bin\ilink32.exe -Le:\Borland\bcc55\lib find03.obj+ c0x32.obj+ hash.obj+ conv.obj /Gn/c/x/ap/Tpe import32.lib+ cw32.lib
if exist *.obj del *.obj
if exist find03.tds del find03.tds
