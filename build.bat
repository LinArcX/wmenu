@echo off
call "C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\Common7\Tools\VsDevCmd.bat"
cl /Zi /Release main.c user32.lib

REM cl.exe /EHsc /Zi /Release main.c
REM "C:\Program Files (x86)\Windows Kits\10\Debuggers\x64\windbg.exe" -srcpath D:\workspace\c\03_fun\winTest -o -G main.exe
