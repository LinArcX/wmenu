@echo off

setlocal EnableDelayedExpansion

REM Use type command to read the content of apps.txt and pipe it to fzf
for /f "delims=" %%a in ('type %CD%\p.txt ^| fzf --header="project:"' ) do ( set "selected=%%a" )
 
if "!selected!"=="build" ( 
  REM Prepare environment to recognize VisualStudio Build Tools
  call "C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\Common7\Tools\VsDevCmd.bat"

  REM build the project using msbuild
  msbuild .
)

if "!selected!"=="debug" ( 
  if exist "%CD%\build\debug\x64\wmenu.exe" (
    start /B .\build\debug\x64\wmenu.exe -elements "foo,bar,zoo" -prompt "Jungle: " -caseInsensitive | more
  ) else (
    ECHO there is no: build\debug\x64\wmenu.exe
  )
)

if "!selected!"=="run" ( 
  if exist "%CD%\build\debug\x64\wmenu.exe" (
    start /B .\build\debug\x64\wmenu.exe -elements "foo,bar,zoo" -prompt "Jungle: " -caseInsensitive | more
  ) else (
    ECHO there is no: build\debug\x64\wmenu.exe
  )
)

if "!selected!"=="clean" (
  rm -rfd build\debug\x64\
)

endlocal
