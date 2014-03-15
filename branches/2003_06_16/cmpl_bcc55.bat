@echo off
if exist findw.exe del findw.exe
e:\Borland\bcc55\Bin\bcc32.exe -D_MSDOS_ -Ie:\Borland\bcc55\Include -c analiz.c bintree.c dict.c findw.c fnmatch.c hash.c list.c wfiles.c conv.c
e:\Borland\bcc55\Bin\ilink32.exe -Le:\Borland\bcc55\lib findw.obj+ c0x32.obj+ analiz.obj+ bintree.obj+ dict.obj+ fnmatch.obj+ hash.obj+ list.obj+ wfiles.obj+ conv.obj /Gn/c/x/ap/Tpe import32.lib+ cw32.lib
if exist *.obj del *.obj
if exist findw.tds del findw.tds
