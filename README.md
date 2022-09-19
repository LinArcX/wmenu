# wmenu
A [dmenu](https://tools.suckless.org/dmenu/) clone for windows operating system written with pure win32 API.

# Why?
When I met dmenu for the first time, I said to myself this simple tool can change my workflow, And surprisingly it did!
I started to use dmenu to automate almost anything in my computer.
From searching through applications, managing software(installing/deleting/updating), running custom scripts, browsing through my favorite directories, playing music, etc..

But the problem is that dmenu uses Xlib under the hood which is not available on windows. There are some hacky ways like compiling dmenu using cygwin, but.. no.. I didn't like that way.

So I started from scratch to clone dmenu using win32 API, without any emulation. This is how wmenu was born :)

# Prerequisites
- MSVC

# Tips
Sicne wmenu uses WINDOWS subsystem when compiling, you can't use stdout directly.

So you should call wmenu like this in your scripts: 

`.\wmenu.exe | more`

# Usage

Let's create a file called `apps.bat` and paste these lines into it:
```
@echo off
start /B wmenu.exe --elements "notepad;explorer;paint;" | more > %temp%/apps.txt

for /f "tokens=*" %%s in (%temp%\apps.txt) do (
  IF "%%s"=="notepad" (
	start %%s
  ) ELSE IF "%%s"=="explorer" (
	start %%s
  ) ELSE IF "%%s"=="paint" (
	start mspaint
  ) ELSE (
	REM default case...
  )
)

del "%temp%\apps.txt"
```

You can use [clavier+](https://github.com/guilryder/clavier-plus) to bind it to a key.

As i said, i [use](https://github.com/LinArcX/winconf/tree/master/wmenu)wmenu for almost anything. Enjoy hacking your workflow :)

## License
![License](https://img.shields.io/github/license/LinArcX/wmenu.svg)
