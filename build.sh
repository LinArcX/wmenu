#!/bin/bash

/usr/bin/x86_64-w64-mingw32-gcc -municode -o output/main.exe main.c
#file output.exe
wine ./output/main.exe
