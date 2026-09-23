# wmenu
A [dmenu](https://tools.suckless.org/dmenu/) clone for the Windows operating system written in pure win32 API.

# shots
[Application launcher](https://github.com/LinArcX/winconf/blob/master/batch/fzf/a.bat):
![application_launcher](https://user-images.githubusercontent.com/10884422/193086477-edc60bb8-2fbc-4974-be43-adb5a9079d20.jpg)

[Jump to favorite directories](https://github.com/LinArcX/winconf/blob/master/batch/fzf/d.bat):
![jump_to_favorite_dirs](https://user-images.githubusercontent.com/10884422/193086873-353ad517-06aa-4669-bc90-b14d044cffcb.jpg)

[PowerManager](https://github.com/LinArcX/winconf/blob/master/batch/wmenu/power_manager.bat):
![power_manager](https://user-images.githubusercontent.com/10884422/193086890-9e3e20ce-2263-4251-aadb-553dc9c276d6.jpg)

# build/debug/run
## `p.bat`
please install these first:
  - fzf
  - windgb
  - Build Tools for Visual Studio 2022.

Then simply run `./p`. You'll see a menu with these items
  - build
  - debug
  - clean
  - run
  - ...

### Debugging
You can use winDbg for debugging wmenu. Eigher install Windows Driver Kit(WDK) or [newer winDbg](https://learn.microsoft.com/en-us/windows-hardware/drivers/debugger/).
  - Do not forget to add the executable of winDbg to your PATH environment variable. (`p.bat` will use it!)
  - If you are on windows 10, you need to be sure that you have Windows 10 Anniversary Update (version 1607) or newer.
    You can install it with choco:
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
- Simply open `.sln` with VisualStudio.

# How it works?
wmenu as its core is a "GUI echo" utility.
If you pass a list of items to it via: `-elements` option, wmenu shows you a ComboBox and and you're able to select one of them.
After that, it echos back that item on your terminal.
If you omit `-elements`, wmenu starts acting like a GUI TextBox.
Now you can type something and by pressing ENTER, it will echo the content of TextBox on the screen.

Since wmenu uses the WINDOWS subsystem when compiling, you can't use stdout directly.

So you should pipe it with `more` command:

  `wmenu.exe -elements "wmenu,dmenu,dwm" | more`

Or if you want to use a specific delimiter:

  `wmenu.exe -element-delimiter ":" -elements "wmenu:dmenu:dwm" | more`

For more information about how using wmenu:
  `wmenu.exe -help | more`

## Some ideas about how to make .bat files
- PowerManagment:
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

- System Application Launcher
```
@echo off

set apps=powershell(pwsh),cmd,paint,regedit,^
window detective,^
system properties,system information,system configuration,control panel,programs and features,services,network adapters,date and time,^
power options,resource monitor,performance monitor,remote desktop,component services,event viewer,firewall,local users and groups manager,device manager,mouse settings,computer managment,advanced user accounts,group policy editor(gpedit.msc),malicious software removal tool

start /B wmenu.exe -elements "%apps%" -prompt "Apps: " -fontName "Cascadia Code PL" -fontSize 9 -caseInsensitive -lineNumber 10 | more > %temp%/apps.txt

for /f "tokens=*" %%s in (%temp%\apps.txt) do (
  if "%%s"=="powershell(pwsh)" ( start pwsh )
  if "%%s"=="cmd" ( start %%s )
  if "%%s"=="paint" ( start mspaint )
  if "%%s"=="regedit" ( regedit )

  if "%%s"=="window detective" ( "C:\Program Files (x86)\Window Detective\Window Detective.exe" )

  if "%%s"=="system properties" ( start sysdm.cpl )
  if "%%s"=="system information" ( start msinfo32 )
  if "%%s"=="system configuration" ( start msconfig ) 
  if "%%s"=="control panel" ( start control )
  if "%%s"=="programs and features" ( start appwiz.cpl )
  if "%%s"=="services" ( start services.msc )
  if "%%s"=="network adapters" ( start ncpa.cpl )
  if "%%s"=="date and time" ( start timedate.cpl ) 

  if "%%s"=="power options" ( start powercfg.cpl)
  if "%%s"=="resource monitor" ( start resmon)
  if "%%s"=="performance monitor" ( start perfmon.msc)
  if "%%s"=="remote desktop" ( start mstsc)
  if "%%s"=="component services" ( start dcomcnfg)
  if "%%s"=="event viewer" ( start eventvwr.msc)
  if "%%s"=="firewall" ( start firewall.cpl)
  if "%%s"=="local users and groups manager" ( start lusrmgr.msc)
  if "%%s"=="device manager" ( start devmgmt.msc)
  if "%%s"=="mouse settings" ( main.cpl )
  if "%%s"=="computer managment" ( start compmgmt.msc)
  if "%%s"=="advanced user accounts" ( start netplwiz)
  if "%%s"=="group policy editor(gpedit.msc)" ( gpedit.msc )
  if "%%s"=="malicious software removal tool" ( start mrt )
)
del "%temp%\apps.txt"

```

You can use [clavier+](https://github.com/guilryder/clavier-plus) to bind it to a key.
Enjoy hacking your workflow :)

## License
![License](https://img.shields.io/github/license/LinArcX/wmenu.svg)
