#include "commandLine.hpp"

#include <sstream>
#include <iostream>

#include "../gui/gui.hpp"
#include "../util/util.hpp"
#include "../commandLine/options/prompt/prompt.hpp"

void CommandLine::parseArguments()
{
  size_t argc = __argc;
  char** argv = __argv;
  for (size_t i = 0; i < argc; ++i)
  {
    if (0 == strcmp(argv[i], "-elements"))
    {
      ComboBox::instance()->items.exists = true;
      ComboBox::instance()->items.rawItems = argv[i + 1];

      std::string item;
      std::stringstream ss (ComboBox::instance()->items.rawItems);
      while (getline (ss, item, ';'))
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
    else if (0 == strcmp(argv[i], "-version"))
    {
      printf("version: %s\n", Gui::instance()->version.c_str());
      SendMessage(Gui::instance()->hwnd, WM_CLOSE, 0, 0);
    }
    else
    {
      //printf("*Warning* Unknow argument: %s\n", argv[i + 1]);
    }
  }
}
