@echo off
if exist findw_t.exe del findw_t.exe
e:\cygwin\bin\gcc -D_MSDOS_ -Wl,-s -O -o findw_t findw.c analiz.c dict.c wfiles.c hash.c list.c bintree.c fnmatch.c conv.c
