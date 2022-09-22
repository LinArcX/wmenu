# wmenu
A [dmenu](https://tools.suckless.org/dmenu/) clone for windows operating system written with pure win32 API.

# Demo

[Application launcher](https://github.com/LinArcX/winconf/blob/master/wmenu/02_apps.bat):
![application_launcher](https://user-images.githubusercontent.com/10884422/190995855-8fda3c72-1f08-4fa5-b454-7a5c29b95507.jpg)

[Jump to favorite directories](https://github.com/LinArcX/winconf/blob/master/wmenu/01_dirs.bat):
![jump_to_favorite_dirs](https://user-images.githubusercontent.com/10884422/190996240-65d5f85c-62a2-4884-b8e8-9dc76f8c873b.jpg)

[PowerManager](https://github.com/LinArcX/winconf/blob/master/wmenu/00_power_manager.bat):
![jump_to_favorite_dirs](https://user-images.githubusercontent.com/10884422/191008799-d19962ec-af4f-47eb-bd32-ef533af7c66b.jpg)

# Why?
1. The main reason is that: I missed my Linux workflow.

	When I met dmenu for the first time, I said to myself: "this simple tool can change your workflow", And surprisingly it did!
	I started to use dmenu to automate almost everything in my computer.
	From searching through applications, managing software(installing/deleting/updating), running custom scripts, browsing through my favorite directories, playing music, etc...

	But the problem is that dmenu uses Xlib under the hood which is not available on windows. There are some hacky ways like compiling dmenu using cygwin, but.. no.. I didn't like that way.

	So I started from scratch to clone dmenu using win32 API, without any emulation. This is how wmenu was born :)

2. The second reason is that I wanted to encourage other people to challenge their habits.
	Microsoft tried so hard during last years(And they are trying right now more than before) to force people to use their operating system in a specific way.
	They pay huge money to their engineers to make for you a __default__ environment.
	They always wanted you to follow their rules and tools. They think instead of you. And they design the house for you.

	But hey, it's the computer world. No one can't tell you __HOW__ you can use your computer.

# Prerequisites
- MSVC

# Tips
Sicne wmenu uses WINDOWS subsystem when compiling, you can't use stdout directly.

So you should call wmenu like this in your scripts:

`wmenu.exe -elements "foo;bar;" | more`

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
1. I intentionally changed/removed some policies that are common in the UNIX world (you can see them in most Linux apps like dmenu).
2. I don't like passing a list of elements by piping(something used in suckless dmenu). Instead, I prefer to __explicitly__ pass them via `-elmenets` option.
3. I'm not a big fan of having two kinds of option names(short names like `-e` and long ones like `--elements`). Instead, I prefer to use full and descriptive names. (`-elements`)
4. I prefer __modularity__ and __readability__ over minimality. Consequently, I don't care about the line of codes!
 I can show you a lot of open source programs that are minimal in lines of code, but they are spaghetti and hard to read or change.

## License
![License](https://img.shields.io/github/license/LinArcX/wmenu.svg)
