@echo off
if exist findw_dj.exe del findw_dj.exe
e:\fsf\bin\gcc -D_MSDOS_ -Wl,-s -O -o findw_dj findw.c analiz.c dict.c wfiles.c hash.c list.c bintree.c fnmatch.c conv.c
rem djgpp не чистит за собой:
if exist findw_dj del findw_dj
