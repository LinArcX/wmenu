# wmenu
A [dmenu](https://tools.suckless.org/dmenu/) clone for the Windows operating system written with pure win32 API.

wmenu is a hybrid GUI/terminal application inspired by [dmenu](https://tools.suckless.org/dmenu/). But it's more than that. It's better to see wmenu as "a GUI echo" utility.

If you pass a list of items to it via: `-elements` option, wmenu shows you a ComboBox and and you're able to select one of them. After that, it echos back that item on your terminal.

If you omit `-elements`, wmenu starts acting like a GUI TextBox. Now you can type something and by pressing ENTER, it will echo the content of TextBox on the screen.

# Demo

[Application launcher](https://github.com/LinArcX/winconf/blob/master/wmenu/02_apps.bat):
![application_launcher](https://user-images.githubusercontent.com/10884422/193086477-edc60bb8-2fbc-4974-be43-adb5a9079d20.jpg)

[Jump to favorite directories](https://github.com/LinArcX/winconf/blob/master/wmenu/01_dirs.bat):
![jump_to_favorite_dirs](https://user-images.githubusercontent.com/10884422/193086873-353ad517-06aa-4669-bc90-b14d044cffcb.jpg)

[PowerManager](https://github.com/LinArcX/winconf/blob/master/wmenu/00_power_manager.bat):
![power_manager](https://user-images.githubusercontent.com/10884422/193086890-9e3e20ce-2263-4251-aadb-553dc9c276d6.jpg)

# Why?
1. The main reason is that: I missed my Linux workflow.

	When I met dmenu for the first time, I said to myself: "This simple tool can change my workflow completely", And surprisingly it did!
	I started to use dmenu to automate almost everything in my machine.
	From searching through applications, managing software(installing/deleting/updating), running custom scripts, browsing through my favorite directories, playing music, etc...

	But the problem is that dmenu uses Xlib under the hood which is not available on Windows. There are some hacky ways like compiling dmenu using Cygwin, but.. no. I didn't like those ways.

	So I started from scratch to clone dmenu using win32 API, without any emulation. This is how wmenu was born :)

2. The second reason is that I wanted to encourage people to challenge their habits. (even if you are a programmer or not.)
	Microsoft tried so hard during the last years(And they are trying now more than before) to force people to use Windows in a specific way.
	They pay huge amounts of money to engineers to create a palace!(__default__ environment)
	In the end, you will become a good user(follower!) But man! it's the computer world. No one can tell you __HOW__ you can use your computer.

# Prerequisites
- MSVC

# How use it?
Since wmenu uses the WINDOWS subsystem when compiling, you can't use stdout directly.

So you should pipe it with `more` command:

`wmenu.exe -elements "wmenu,dmenu,dwm" | more`

Or if you want to use a specific delimiter:

`wmenu.exe -element-delimiter ":" -elements "wmenu:dmenu:dwm" | more`


For more information about how using wmenu:
`wmenu.exe -help | more`

# Usage

Let's create a file called `apps.bat` and paste these lines into it:
```
@echo off
start /B wmenu.exe -elements "notepad;explorer;paint;" | more > %temp%/apps.txt

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

As i said, i [use](https://github.com/LinArcX/winconf/tree/master/wmenu) wmenu for almost anything. Enjoy hacking your workflow :)

# Philosophy behind code structure
1. I intentionally changed/removed some policies that are common in the UNIX world (you can see them in most Linux software like dmenu).
2. I don't like passing a list of elements by piping(something used in suckless dmenu). Instead, I prefer to __explicitly__ pass them via `-elements` option.
3. I'm not a big fan of having two kinds of option names(short names like `-e` and long ones like `--elements`). Instead, I prefer to use full and descriptive names. (`-elements`)

## License
![License](https://img.shields.io/github/license/LinArcX/wmenu.svg)
