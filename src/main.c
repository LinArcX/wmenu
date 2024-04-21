//#include "gui.hpp"

//#include <sstream>
//#include <iostream>

//#include "../util/util.hpp"
//#include "../commandLine/commandLine.hpp"
//#include "../commandLine/options/prompt/prompt.hpp"
////std::wstring stringToLpcwstr(std::string source)
//{
//  std::wstring temp = std::wstring(source.begin(), source.end());
//  return temp;
//}

#pragma warning(disable: 4996)

char*
getline_custom(FILE *file) 
{
  char *line = NULL;
  size_t bufsize = 0;
  ssize_t bytes_read;

  bytes_read = getline(&line, &bufsize, file);

  if (bytes_read == -1) {
      free(line);
      return NULL;
  } else {
      // Remove newline character if present
      if (bytes_read > 0 && line[bytes_read - 1] == '\n') {
          line[bytes_read - 1] = '\0';
      }
      return line;
  }
}

struct Prompt
{
  bool exists = false;
  wchar_t value[300] = NULL;
  size_t length = 0;
}




LPWSTR charPtrToLpcwstr(const char* source)
{
  wchar_t wtext[500];
  mbstowcs(wtext, source, strlen(source) + 1);
  LPWSTR ptr = wtext;
  return ptr;
}

int stringToInt(std::string input)
{
  int output;
  std::stringstream stream(input);
  stream >> output;
  return output;
}

size_t findSubStrinInString(std::string data, std::string subString, size_t pos)
{
  std::transform(data.begin(), data.end(), data.begin(), ::tolower);
  std::transform(subString.begin(), subString.end(), subString.begin(), ::tolower);
  return data.find(subString, pos);
}

std::wstring stringToWString(const std::string& str)
{
  int size_needed = MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), NULL, 0);
  std::wstring wstrTo( size_needed, 0 );
  MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), &wstrTo[0], size_needed);
  return wstrTo;
}

bool isDelimiterValid(char delimiter)
{
  bool isValid = false;
  switch (delimiter)
  {
  case ',':
  case ';':
  case ':':
  case 32: // SPACE
  case '-':
  case '_':
  case '|':
    isValid = true;
    break;
  default:
    isValid = false;
    break;
  }
  return isValid;
}

bool containsCarriageOrTab(char raw)
{
  bool result = false;
  switch (raw)
  {
  case 'n':
  case 'r':
  case 't':
    result = true;
    break;
   default:
     result = false;
    break;
  }
  return result;
}

char convertCarriageOrTabToAscii(char raw)
{
  char result;
  switch (raw)
  {
  case 'n':
    result = 10;
    break;
  case 'r':
    result = 13;
    break;
  case 't':
    result = 9;
    break;
   default:
    result = raw;
    break;
  }
  return result;
}

int parseArguments()
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
      combo->items.exists = true;
      combo->items.rawItems = argv[i + 1];

      char* item = NULL;
      //std::string item;
      std::stringstream ss (ComboBox::instance()->items.rawItems);
      while (getline (ss, item, delimiter))
      {
        //ComboBox::instance()->items.listOfItems.push_back(item);
        strings = (char **)realloc(strings, (size + 1) * sizeof(char *));
        strings[size++] = strdup("Hello");
      }
    }
    else if (0 == strcmp(argv[i], "-prompt"))
    {
      prompt->exists = true;
      prompt->value = Util::stringToWString(argv[i + 1]);
      prompt->length = Prompt::instance()->value.length();
    }
    else if (0 == strcmp(argv[i], "-caseInsensitive"))
    {
      ComboBox::instance()->caseSensitive.exists = false;
    }
    //else if (0 == strcmp(argv[i], "-position"))
    //{
    //  ComboBox::instance()->position.x = false;
    //}
    else if (0 == strcmp(argv[i], "-fontName"))
    {
      font->name.exists = true;
      font->name.value = argv[i + 1];
    }
    else if (0 == strcmp(argv[i], "-fontSize"))
    {
      font->size.exists = true;
      font->size.value = (Util::stringToInt(argv[i + 1]));
    }
    else if (0 == strcmp(argv[i], "-lineNumber"))
    {
      combo->lineNumber.exists = true;
      combo->lineNumber.value = Util::stringToInt(argv[i + 1]);
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
    \tBut it is more than dmenu. It's better to see it as \"a GUI echo\" utility.\n\n \\
    \tIf you pass it a list of items via: `-elements` option, \n \\
    \twmenu shows you a ComboBox and and can choose one of the items from the list. After that, it echos back that item on your terminal.\n\n \\
    \tIf you omit `-elements`, wmenu starts acting like a GUI TextBox.\n \\
    \tNow you can type something and by pressing ENTER, it will echo the content of TextBox on the screen.\n \\
    \n\n");

  printf("OPTIONS\n");
  printf("\t -elements\n");
  printf("\t\tWith this option, you can pass a list of comma-separated items to wmenu. example: `wmenu -elements \"foo,bar,baRR,BAr,For,Foo,FOOO,BAZZ\"`\n\n");

  printf("\t -element-delimiter\n");
  printf("\t\tSpecify which character to seperate the elements from each other.[valid delimiters: , ; : - _ | And SPACE] If the option is ommited, wmenu will use \",\" by default\
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
  printf("\t\tShows version number.\n\n");

  printf("\t -help\n");
  printf("\t\tShows this help page.\n\n");
}

LRESULT CALLBACK TextBoxWindowHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
  switch (msg)
  {
  case WM_KEYDOWN:
  {
    switch (wParam)
    {
    case VK_RETURN:
    {
      wchar_t buf[256];
      SendMessage(hwnd, (UINT)EM_GETLINE, (WPARAM)0, (LPARAM)buf);
      printf("%ls", buf);
      SendMessage(hwnd, WM_CLOSE, 0, 0);
    }
    }
  } break;
  case WM_CLOSE:
  {
    PostQuitMessage(0);
    return 0;
  } break;
  }
  return CallWindowProc(TextBox::window_proc, hwnd, msg, wParam, lParam);
}

void createTextBox(HWND hwnd, int x, int y, int width, int height, char* fontName)
{
   HWND hwndTextBox = CreateWindow(WC_EDIT, L"",
     WS_BORDER | WS_CHILD | WS_VISIBLE | WS_VISIBLE | ES_WANTRETURN,
     x, y, width, height,
     hwnd, (HMENU)hwnd_id, NULL, NULL);

  if (Font::instance()->name.exists)
  {
    Util::setFont(hwndTextBox, fontName,
      Font::instance()->size.exists ? (Font::instance()->size.value * 2) + (Font::instance()->size.value / 2) : 20,
      Font::instance()->size.exists ? Font::instance()->size.value : 8);
  }

  window_proc = (WNDPROC) SetWindowLongPtr(hwndTextBox, GWLP_WNDPROC, (LONG_PTR)TextBoxWindowHandler);
  SetFocus(hwndTextBox);
}

void createLabel(HWND hwnd, int x, int y, int width, int height, char* fontName, std::wstring text)
{
  HWND hwndPrompt = CreateWindow(L"static", NULL, WS_CHILD | WS_VISIBLE,
    x, y, width, height,
    hwnd, NULL,
    (HINSTANCE) GetWindowLongPtr (hwnd, GWLP_HINSTANCE), NULL);

  if (Font::instance()->name.exists)
  {
    Util::setFont(hwndPrompt, fontName,
      Font::instance()->size.exists ? (Font::instance()->size.value * 2) + (Font::instance()->size.value / 2) : 20,
      Font::instance()->size.exists ? Font::instance()->size.value : 8);
  }

  SetWindowText(hwndPrompt, text.c_str());
}

int setLineNumber()
{
  int output = 0;
  if (1 == lineNumber.value) { output =  50; }
  else if (2 == lineNumber.value) { output =  85; }
  else if (3 == lineNumber.value) { output = 105; }
  else { output = 105 + ((lineNumber.value - 3) * 20); }
  return output;
}

void setupGui(HWND m_hwnd)
{
  if (ComboBox::instance()->items.exists)
  {
    if (Prompt::instance()->exists)
    {
      label.createLabel(m_hwnd,
        0,
        0,
        (Prompt::instance()->length * 8),
        155,
        Font::instance()->name.value,
        Prompt::instance()->value);

      int lineNumber = 0;
      if (ComboBox::instance()->lineNumber.exists)
      {
        lineNumber = setLineNumber();
      }
      else
      {
        lineNumber = 125;
      }

      ComboBox::instance()->createComboBox(m_hwnd,
        Prompt::instance()->exists ? Prompt::instance()->length * 8 : 0,
        0,
        width - (Prompt::instance()->length * 8),
        lineNumber,
        Font::instance()->name.value);
    }
    else
    {
      ComboBox::instance()->createComboBox(hwnd,
        Prompt::instance()->exists ? Prompt::instance()->length * 8 : 0,
        0,
        Prompt::instance()->exists ? (width - (Prompt::instance()->length * 8)) : width,
        ComboBox::instance()->lineNumber.exists ? ComboBox::instance()->lineNumber.value : 125,
        Font::instance()->name.value);
    }
  }
  else
  {
    if (Prompt::instance()->exists)
    {
      label.createLabel(m_hwnd,
        0,
        0,
        (Prompt::instance()->length * 8),
        125,
        Font::instance()->name.value,
        Prompt::instance()->value);

      textBox.createTextBox(m_hwnd,
        Prompt::instance()->exists ? Prompt::instance()->length * 8 : 0,
        0,
        Prompt::instance()->exists ? width - Prompt::instance()->length : width,
        200,
        Font::instance()->name.value);
    }
    else
    {
      textBox.createTextBox(m_hwnd,
        Prompt::instance()->exists ? Prompt::instance()->length * 8 : 0,
        0,
        Prompt::instance()->exists ? width - Prompt::instance()->length : width,
        200,
        Font::instance()->name.value);
    }
  }
}

bool
isRunningCheck() 
{
  std::wstring suffix = L"_Mutex";
  std::wstring fullName = name + suffix;
  HANDLE Mutex = CreateMutex(NULL, FALSE, fullName.c_str());
  if (ERROR_ALREADY_EXISTS == GetLastError())
  {
    return true;
  }
  else
  {
    return false;
  }
}

LRESULT CALLBACK WindowHandler(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, Gui& gui)
{
  HDC hdc;
  PAINTSTRUCT ps;
  switch (message)
  {
  case WM_ACTIVATE:
  case WM_SETFOCUS:
  case WM_ENABLE:
  case WM_MOVE:
  case WM_FONTCHANGE:
  {
    HDC hdcStatic = (HDC)wParam;
    SetTextColor(hdcStatic, RGB(240, 216, 192));
    SetBkColor(hdcStatic, RGB(79, 78, 77));
    return (INT_PTR)CreateSolidBrush(RGB(107, 105, 99));
  } break;
  case WM_PAINT:
  {
    hdc = BeginPaint(hWnd, &ps);
    EndPaint(hWnd, &ps);
    return 0;
  } break;
  case WM_COMMAND:
  {
    if (HIWORD(wParam) == CBN_SELENDOK)
    {
      if (LOWORD(wParam) == ComboBox::instance()->hwnd_id)
      {
        HWND hcombo = (HWND)lParam;
        LRESULT index = SendMessageW(hcombo, CB_GETCURSEL, (WPARAM)0, (LPARAM)0);
        wchar_t buf[256];
        SendMessageW(hcombo, (UINT)CB_GETLBTEXT, (WPARAM)index, (LPARAM)buf);
        printf("%ls", buf);
        SendMessage(hWnd, WM_CLOSE, 0, 0);
      }
    }
    if (HIWORD(wParam) == CBN_EDITUPDATE)
    {
      static WCHAR str[128];
      if (LOWORD(wParam) == ComboBox::instance()->hwnd_id)
      {
        HWND hcombo = (HWND)lParam;
        GetWindowText(hcombo, str, 128);
        std::vector<std::string> _tempList;
        //ComboBox_SelectString(hwndComboBox, 0, L"sa");

        for (size_t i = 0; i < ComboBox::instance()->items.listOfItems.size(); ++i)
        {
          // Delete all items
          std::wstring stemp = std::wstring(ComboBox::instance()->items.listOfItems[i].begin(), ComboBox::instance()->items.listOfItems[i].end());
          LPCWSTR sw = stemp.c_str();
          SendMessageW(ComboBox::instance()->hwnd, CB_DELETESTRING, 0, (LPARAM)sw);

          // Add new items to vector
          std::wstring ws(str);
          std::string test(ws.begin(), ws.end());

          size_t pos = 0;
          if (!ComboBox::instance()->caseSensitive.exists)
          {
            pos = Util::findSubStrinInString(ComboBox::instance()->items.listOfItems[i], test);
           }
          else
          {
            pos = ComboBox::instance()->items.listOfItems[i].find(test, 0);
          }

          if (std::string::npos != pos)
          {
            _tempList.push_back(ComboBox::instance()->items.listOfItems[i]);
          }
        }

        for (size_t i = 0; i < _tempList.size(); ++i)
        {
          std::wstring stemp = std::wstring(_tempList[i].begin(), _tempList[i].end());
          LPCWSTR sw = stemp.c_str();
          SendMessageW(ComboBox::instance()->hwnd, CB_ADDSTRING, 0, (LPARAM)sw);
        }
        ComboBox_ShowDropdown(ComboBox::instance()->hwnd, TRUE);
        //ComboBox_SetCurSel(hwndComboBox, 1);
      }
    }
  } break;
  case WM_CTLCOLORMSGBOX:
  case WM_CTLCOLORLISTBOX:
  case WM_CTLCOLORBTN:
  case WM_CTLCOLORDLG:
  case WM_CTLCOLORSCROLLBAR:
  case WM_CTLCOLOREDIT:
  {
    HDC hdcStatic = (HDC)wParam;
    SetTextColor(hdcStatic, RGB(240, 216, 192));
    SetBkColor(hdcStatic, RGB(79, 78, 77));
    return (INT_PTR)CreateSolidBrush(RGB(107, 105, 99));
  } break;
  case WM_CTLCOLORSTATIC:
  {
    HDC hdcStatic = (HDC)wParam;
    SetTextColor(hdcStatic, RGB(240, 216, 192));
    SetBkColor(hdcStatic, RGB(79, 78, 77));
    return (INT_PTR)CreateSolidBrush(RGB(107, 105, 99));
  } break;
  case WM_CLOSE:
  {
    PostQuitMessage(0);
    return 0;
  } break;
  default:
  {
    return DefWindowProcW(hWnd, message, wParam, lParam);
  } break;
  }
  return DefWindowProc(hWnd, message, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
  name = L"wmenu";
  version = "1.1.0";
  width = GetSystemMetrics(SM_CXSCREEN);
  height = GetSystemMetrics(SM_CYSCREEN);

  INT Result = 0;
  if (!isRunningCheck())
  {
    CommandLine commandLine;
    Result = commandLine.parseArguments();
    if (Result)
    {
      return Result;
    }

    MSG msg;
    WNDCLASS wndclass = { 0 } ;

    wndclass.cbClsExtra = 0;
    wndclass.cbWndExtra = 0;
    wndclass.lpszMenuName = NULL;
    wndclass.hInstance = hInstance;
    wndclass.lpfnWndProc = (WNDPROC) WindowHandler;
    wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
    wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wndclass.style = CS_HREDRAW | CS_VREDRAW | CS_NOCLOSE;
    wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wndclass.lpszClassName = name.c_str();

    if (!RegisterClass(&wndclass))
    {
      MessageBox(NULL, TEXT("This program requires Windows NT!"), name.c_str(), MB_ICONERROR);
      return -1;
    }

    hwnd = CreateWindow(name.c_str(),
      name.c_str(),
      WS_POPUP | WS_SYSMENU,
      0,                                                    // initial x position
      0,                                                    // initial y position
      width,                                                // initial width
      ComboBox::instance()->items.exists ? height / 5 : 20, // initial height
      NULL,                                                 // parent window handle
      NULL,                                                 // window menu handle
      hInstance,                                            // program instance handle
      NULL);                                                // creation parameters

    if (NULL == hwnd)
    {
      DWORD error = ::GetLastError();
      std::string message = std::system_category().message(error);
      printf("ERROR: %s", message.c_str());
      return -1;
    }

    setupGui(hwnd);
    ShowWindow(hwnd, iCmdShow);
    UpdateWindow(hwnd);

    while (GetMessage(&msg, NULL, 0, 0))
    {
      TranslateMessage(&msg);
      DispatchMessage(&msg);

      switch (msg.message)
      {
      case WM_KEYDOWN:
      {
        if ((GetAsyncKeyState(VK_ESCAPE) & 0x01))
          {
            SendMessage(hwnd, WM_CLOSE, 0, 0);
          }
      } break;
      }
    }
    Result = msg.wParam;
  }
  return Result;
}

//#include "gui/gui.h"
//int Gui::initialize(const HINSTANCE hInstance, int iCmdShow)
  //return Gui::instance()->initialize(hInstance, iCmdShow);
//}
