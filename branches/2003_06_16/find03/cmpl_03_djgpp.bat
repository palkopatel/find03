@echo off
del find03.exe
e:/fsf/bin/gcc -D_MSDOS_ -O -o find03 find03.c hash.c ../conv.c
rem djgpp не чистит за собой:
rem del find03
