@echo off
del find03.exe
e:\cygwin\bin\gcc -D_MSDOS_ -Wl,-s -ofind03 find03.c hash.c ../conv.c
