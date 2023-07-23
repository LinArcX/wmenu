#include "commandLine.hpp"

#include <sstream>
#include <iostream>

#include "../gui/gui.hpp"
#include "../util/util.hpp"
#include "../commandLine/options/prompt/prompt.hpp"

int CommandLine::parseArguments()
{
  int exitFlag = 0;
  ComboBox::instance()->caseSensitive.exists = true;
  size_t argc = __argc;
  char** argv = __argv;
  char delimiter = ',';

  for (size_t i = 0; i < argc; ++i)
  {
    if (0 == strcmp(argv[i], "-version"))
    {
      printf("version: %s\n", Gui::instance()->version.c_str());
      SendMessage(Gui::instance()->hwnd, WM_CLOSE, 0, 0);
      exitFlag = -1;
      break;
    }
    else if (0 == strcmp(argv[i], "-help"))
    {
      help();
      SendMessage(Gui::instance()->hwnd, WM_CLOSE, 0, 0);
      exitFlag = -1;
      break;
    }
    else if (0 == strcmp(argv[i], "-element-delimiter"))
    {
      std::string delimiterRaw = argv[i + 1];
      if (delimiterRaw.length() == 2 && (delimiterRaw[0] == '\\' && Util::containsCarriageOrTab(delimiterRaw[1])))
      {
        delimiter = Util::convertCarriageOrTabToAscii(delimiterRaw[1]);
      }
      else if(delimiterRaw.length() == 1 && Util::isDelimiterValid(delimiterRaw[0]))
      {
        delimiter = delimiterRaw[0];
      }
      else
      {
        printf("Delimiter must be a single and valid character. [, ; : - _ | And SPACE]\n");
        SendMessage(Gui::instance()->hwnd, WM_CLOSE, 0, 0);
        exitFlag = -1;
        break;
      }
    }
    else if (0 == strcmp(argv[i], "-elements"))
    {
      ComboBox::instance()->items.exists = true;
      ComboBox::instance()->items.rawItems = argv[i + 1];

      std::string item;
      std::stringstream ss (ComboBox::instance()->items.rawItems);
      while (getline (ss, item, delimiter))
      {
        ComboBox::instance()->items.listOfItems.push_back(item);
      }
    }
    else if (0 == strcmp(argv[i], "-prompt"))
    {
      Prompt::instance()->exists = true;
      Prompt::instance()->value = Util::stringToWString(argv[i + 1]);
      Prompt::instance()->length = Prompt::instance()->value.length();
    }
    else if (0 == strcmp(argv[i], "-caseInsensitive"))
    {
      ComboBox::instance()->caseSensitive.exists = false;
    }
    else if (0 == strcmp(argv[i], "-position"))
    {
    }
    else if (0 == strcmp(argv[i], "-fontName"))
    {
      Font::instance()->name.exists = true;
      Font::instance()->name.value = argv[i + 1];
    }
    else if (0 == strcmp(argv[i], "-fontSize"))
    {
      Font::instance()->size.exists = true;
      Font::instance()->size.value = (Util::stringToInt(argv[i + 1]));
    }
    else if (0 == strcmp(argv[i], "-lineNumber"))
    {
      ComboBox::instance()->lineNumber.exists = true;
      ComboBox::instance()->lineNumber.value = Util::stringToInt(argv[i + 1]);
    }
  }
  return exitFlag;
}

void CommandLine::help()
{
  printf("NAME\t%ls\n", Gui::instance()->name.c_str());
  printf("\tA dmenu clone for windows operating system written with pure win32 API\n\n\n");

  printf("SYNOPSIS\n");
  printf("\t wmenu.exe [OPTIONS...] | more\n\n\n");

  printf("DESCRIPTION\n");
  printf("\twmenu is a hybrid gui/terminal win32 application inspired by dmenu from suckless.\n \\
    \tBut it is more than dmenu. It's better to see wmenu as \"a GUI echo\" utility.\n\n \\
    \tIf you pass it a list of items to it via : `-elements` option, \n \\
    \twmenu shows you a ComboBox and and you are able to select one of them. After that, it echos back that item on your terminal.\n\n \\
    \tIf you omit `-elements`, wmenu starts acting like a GUI TextBox.\n \\
    \tNow you can type something and by pressing ENTER, it will echo the content of TextBox on the screen.\n \\
    \n\n");

  printf("OPTIONS\n");
  printf("\t -elements\n");
  printf("\t\tWith this option, you can pass a list of semicolon-separated items to wmenu. example: `wmenu -elements \"foo;bar;baRR;BAr;For;Foo;FOOO;BAZZ\"`\n\n");

  printf("\t -element-delimiter\n");
  printf("\t\tSpecify what character to seperate the elements from each other.[valid delimiters: , ; : - _ | And SPACE] If the option is ommited, wmenu will use \",\" by default\
    \n\t\texample: `wmenu -element-delimiter \":\" -elements \"foo:bar:baRR:BAr:For:Foo:FOOO:BAZZ\"`\n\n");

  printf("\t -prompt\n");
  printf("\t\tShows a label at the top-left corner of wmenu.\n\n");

  printf("\t -caseInsensitive\n");
  printf("\t\tWhen passing a list of items to `-elements`, you can specify the search behavior should be case insensitive.\n\n");

  printf("\t -fontName\n");
  printf("\t\tSpecifying the font name that is used for GUI elements(ComboBox, TextBox, Label, ...).\n\n");

  printf("\t -fontSize\n");
  printf("\t\tSpecifying the font size that is used for GUI elements(ComboBox, TextBox, Label, ...).\n\n");

  printf("\t -lineNumber\n");
  printf("\t\tSpecifying the number of items when showing the comboBox(This option should be used only with `-elements` option).\n\n");

  printf("\t -version\n");
  printf("\t\tShow version number.\n\n");

  printf("\t -help\n");
  printf("\t\tShows this help page.\n\n");
}
