# wmenu
A [dmenu](https://tools.suckless.org/dmenu/) clone for the Windows operating system written in pure win32 API.

# why?
wmenu is a hybrid GUI/terminal application inspired by [dmenu](https://tools.suckless.org/dmenu/). There are some reasons that i started to develop this software:

- I missed my linux workflow.

	When I met dmenu for the first time, I said to myself: "This simple tool can change your workflow completely", And surprisingly it did!
	I started to use dmenu to automate almost everything in my machine.
	From searching through applications, managing software(installing/deleting/updating), running custom scripts, browsing through my favorite directories, playing music, etc...

	But the problem is that dmenu uses Xlib under the hood which is not available on Windows. There are some hacky ways like compiling dmenu using Cygwin, but.. no. I didn't like those ways.

	So I started from scratch to clone dmenu using win32 API, without any emulation. This is how wmenu was born :)

- To notify myself and others that we are the ones who should control the computers, not the opposite.
  
  We can feel that with the rise of AI + Cloud Computing,  we'll no longer understand what's happening inside our machines.

- To show you how we can create a simple and minimal c/win32 application without using visual studio. without using trendy/bubbly ideas like clean code, design patters, etc..
  
  You just need a set of fucking simple tools: shell/editor/compiler/debugger. And those are enough for you and me. :)

- To encourage people to challenge their habits. (even if you are a programmer or not.)

	Microsoft tried so hard during the last years (And they are trying more than before) to force people to use Windows in a specific way.
	They pay huge amounts of money to engineers to create a palace!(__default__ environment)
	At the end, you will become a good user(follower!) But man! it's the computer world. No one can tell you __HOW__ you can use your computer.

# code of conduct
Previously i was using c++, design patterns and following uncle Bob's Clean Code for wmenu. After a lot of experiences, i realized that a source-code can be call __clean__ when:
- it's written in a simplest form even a noob can read and rationalize about it's structure and intention easily.
- we don't overcomplicate things.
  - smart solutions sometimes lead to add more complexities. so: KISS.
- you can debug your code more easily.

# shots
[Application launcher](https://github.com/LinArcX/winconf/blob/master/batch/fzf/a.bat):
![application_launcher](https://user-images.githubusercontent.com/10884422/193086477-edc60bb8-2fbc-4974-be43-adb5a9079d20.jpg)

[Jump to favorite directories](https://github.com/LinArcX/winconf/blob/master/batch/fzf/d.bat):
![jump_to_favorite_dirs](https://user-images.githubusercontent.com/10884422/193086873-353ad517-06aa-4669-bc90-b14d044cffcb.jpg)

[PowerManager](https://github.com/LinArcX/winconf/blob/master/batch/wmenu/power_manager.bat):
![power_manager](https://user-images.githubusercontent.com/10884422/193086890-9e3e20ce-2263-4251-aadb-553dc9c276d6.jpg)

# build/debug/run
## `p.bat`
if you prefer minimal environment and independent tools like me, and don't want to rely on visual studio for building/debugging, please download and install:
  - fzf
  - windgb
  - Build Tools for Visual Studio 2022.

I provide a batch script called: `p.bat` that is using `fzf` and `p.txt` to make a menu for you in cmd. After run it, you can choose to:
- build
- debug
- clean
- run

### Debugging
You can use winDbg for debugging wmenu. eigher install Windows Driver Kit(WDK) or [newer winDbg](https://learn.microsoft.com/en-us/windows-hardware/drivers/debugger/) alone.
  - Not forget to add the executable of winDbg to your PATh environment variable, since p.bat will use it.
  - If you are on windows 10, you need to be sure that you have: Windows 10 Anniversary Update (version 1607) or newer. you can install it with choco:
  `choco install windows-10-update-assistant --version=1.4.9200.22175`

Then just run `p.bat` and select __debug__ option.

### Troubleshooting
#### Error: VsDevCmd.bat encountered errors. Environment may be incomplete and/or incorrect. ** error VsDev.cmd
Open regedit and go to:
	HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Command Processor
Change:
	Autorun = "cls"
To:
	Autorun = ""

## Visual Studio
- If you prefer visual studio, you can just download visual studio and open .sln in the root of the project with it.
  - All the debugging/building/cleaning the project will be happen inside this magic box.

# How it works?
wmenu as it's core is a "GUI echo" utility.
If you pass a list of items to it via: `-elements` option, wmenu shows you a ComboBox and and you're able to select one of them. After that, it echos back that item on your terminal.
If you omit `-elements`, wmenu starts acting like a GUI TextBox. Now you can type something and by pressing ENTER, it will echo the content of TextBox on the screen.

Since wmenu uses the WINDOWS subsystem when compiling, you can't use stdout directly.

So you should pipe it with `more` command:

  `wmenu.exe -elements "wmenu,dmenu,dwm" | more`

Or if you want to use a specific delimiter:

  `wmenu.exe -element-delimiter ":" -elements "wmenu:dmenu:dwm" | more`

For more information about how using wmenu:
  `wmenu.exe -help | more`

Now let's create a file called `apps.bat` and paste these lines into it:
```
@echo off
start /B wmenu.exe -elements "notepad;explorer;paint;" | more > %temp%/apps.txt

for /f "tokens=*" %%s in (%temp%\apps.txt) do (
  if "%%s"=="notepad" ( start %%s)
  else if "%%s"=="explorer" ( start %%s )
  else if "%%s"=="paint" ( start mspaint )
  else ( REM default case...)
)

del "%temp%\apps.txt"
```

You can use [clavier+](https://github.com/guilryder/clavier-plus) to bind it to a key.
As i said, i [use](https://github.com/LinArcX/winconf/tree/master/wmenu) wmenu for almost anything. Enjoy hacking your workflow :)

## License
![License](https://img.shields.io/github/license/LinArcX/wmenu.svg)
