#include <stdio.h>
#include <windows.h>
#include <windowsx.h>
#include <commctrl.h>

#include <list>
#include <vector>
#include <sstream>
#include <iostream>
#include <algorithm>

using namespace std;

#define APP_NAME "wmenu"
std::string version = "1.0.0";
#pragma warning(disable: 4996)

HWND hwnd;
BOOL appIsRunning;
int width, height;
std::list<int> items;
std::vector<std::string> elementList;
static TCHAR wAppName[] = TEXT(APP_NAME);

#define WCOMBO 3
#define WMYTEXT 4

HWND hwndComboBox = NULL;
HWND hwndTextBox = NULL;
WNDPROC lpfnEditWndProc = NULL;

bool bottom = false;

bool hasPrompt = false;
char* prompt = NULL;
size_t promptLength = 0;

bool hasElements = false;
char* elements = NULL;

bool caseSensitive = true;

bool hasFontName = false;
char* fontName = NULL;

bool hasFontSize = false;
int fontSize = 0;

bool hasLineNumber = false;
int lineNumber = 0;

BOOL appAlreadyRunning(void) {
  HANDLE Mutex = NULL;
  Mutex = CreateMutex(NULL, FALSE, APP_NAME L"_Mutex");
  if (GetLastError() == ERROR_ALREADY_EXISTS) {
    return (TRUE);
  } else {
    return (FALSE);
  }
}

void initialize()
{
  width = GetSystemMetrics( SM_CXSCREEN );
  height = GetSystemMetrics( SM_CYSCREEN );
}

void setFont(HWND hwnd, char* fontName)
{
  if (hasFontName)
  {
    wchar_t wtext[400];
    mbstowcs(wtext, fontName, strlen(fontName) + 1);
    LPWSTR ptr = wtext;

    HFONT hFont = CreateFont(
      hasFontSize ? (fontSize * 2) + (fontSize / 2) : 20,
      fontSize ? fontSize : 8,
      0, 0,
      FW_BOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS,
      CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, NULL, ptr);
    SendMessage(hwnd, WM_SETFONT, (WPARAM)hFont, 0);
  }
}

int setLineNumber()
{
   if (1 == lineNumber)
  {
    return 50;
  }
  else if (2 == lineNumber)
  {
    return 85;
  }
  else if (3 == lineNumber)
  {
    return 105;
  }
  else {
    return 105 + (lineNumber - 3) * 20;
  }
}

void createComboBox(HWND hwnd)
{
  hwndComboBox = CreateWindow(WC_COMBOBOX, L"",
    CBS_DROPDOWN | CBS_HASSTRINGS | WS_CHILD | WS_VISIBLE,
    hasPrompt ? promptLength * 8 : 0,
    0,
    hasPrompt ? width - (promptLength*8) : width,
    hasLineNumber ? setLineNumber() : 125,
    hwnd, (HMENU)WCOMBO, NULL, NULL);

  setFont(hwndComboBox, fontName);

  if (elementList.size())
  {
    for (size_t i = 0; i < elementList.size(); ++i)
    {
      std::wstring stemp = std::wstring(elementList[i].begin(), elementList[i].end());
      LPCWSTR sw = stemp.c_str();
      SendMessageW(hwndComboBox, CB_ADDSTRING, 0, (LPARAM)sw);
    }
    ComboBox_ShowDropdown(hwndComboBox, TRUE);

    RECT rect;
    int comboWidth = 0;
    int comboHeight = 0;

    if(GetWindowRect(hwndComboBox, &rect))
    {
      comboWidth = rect.right - rect.left;
      comboHeight = rect.bottom - rect.top;
    }
    SetWindowPos(hwnd, NULL, 0, 0, width, comboHeight, NULL);
    SetFocus(hwndComboBox);
  }
  else
  {
    printf("There's no elmenets!");
  }
}

LRESULT CALLBACK SubClassProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
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
      SendMessage(hwndTextBox, (UINT)EM_GETLINE, (WPARAM)0, (LPARAM)buf);
      printf("%ls", buf);
      SendMessage(hwnd, WM_CLOSE, 0, 0);
    }
    }
  } break;
  case WM_CLOSE:
  {
    appIsRunning = FALSE;
    PostQuitMessage(0);
    return 0;
  } break;
  }
  return CallWindowProc(lpfnEditWndProc, hwnd, msg, wParam, lParam);
}

void createTextBox(HWND hwnd)
{
   hwndTextBox = CreateWindow(WC_EDIT, L"",
     WS_BORDER | WS_CHILD | WS_VISIBLE | WS_VISIBLE | ES_WANTRETURN,
     hasPrompt ? promptLength * 8 : 0,
     0,
     hasPrompt ? width - promptLength : width,
     200,
     hwnd, (HMENU)WMYTEXT, NULL, NULL);

    lpfnEditWndProc = (WNDPROC) SetWindowLongPtr(hwndTextBox, GWLP_WNDPROC, (LONG_PTR) SubClassProc);
    SetFocus(hwndTextBox);
}

void createLabel(HWND hwnd)
{
  HWND hwndPrompt = CreateWindow(L"static", NULL,
    WS_CHILD | WS_VISIBLE,
    0, 0, promptLength * 8, 125,
    hwnd, NULL, //(HMENU)(501),
    (HINSTANCE) GetWindowLong (hwnd, GWL_HINSTANCE), NULL);

  wchar_t wtext[400];
  mbstowcs(wtext, prompt, strlen(prompt) + 1);
  LPWSTR ptr = wtext;

  setFont(hwndPrompt, fontName);
  SetWindowText(hwndPrompt, ptr);
}

void setupGui(HWND hwnd) {
  if (hasElements)
  {
    if (hasPrompt)
    {
      createLabel(hwnd);
      createComboBox(hwnd);
    }
    else
    {
      createComboBox(hwnd);
    }
  }
  else
  {
    if (hasPrompt)
    {
      createLabel(hwnd);
      createTextBox(hwnd);
    }
    else
    {
      createTextBox(hwnd);
    }
  }
}

size_t findSubStrinInString(std::string data, std::string subString, size_t pos = 0)
{
  if (!caseSensitive)
  {
    std::transform(data.begin(), data.end(), data.begin(), ::tolower);
    std::transform(subString.begin(), subString.end(), subString.begin(), ::tolower);
  }
  return data.find(subString, pos);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{
  HDC hdc;
  PAINTSTRUCT ps;
  switch (msg)
  {
    case WM_CREATE:
    {
      setupGui(hWnd);
    } break;
    case WM_PAINT:
    {
      hdc = BeginPaint(hWnd, &ps);
      EndPaint(hWnd, &ps);
      return 0;
    } break;
    case WM_COMMAND:
    {
      if (HIWORD(wp) == CBN_SELENDOK)
      {
        if (LOWORD(wp) == WCOMBO)
        {
          HWND hcombo = (HWND)lp;
          LRESULT index = SendMessageW(hcombo, CB_GETCURSEL, (WPARAM)0, (LPARAM)0);
          wchar_t buf[256];
          SendMessageW(hcombo, (UINT)CB_GETLBTEXT, (WPARAM)index, (LPARAM)buf);
          printf("%ls", buf);
          SendMessage(hwnd, WM_CLOSE, 0, 0);
        }
      }
      if (HIWORD(wp) == CBN_EDITUPDATE)
      {
        static WCHAR str[128];
        if (LOWORD(wp) == WCOMBO)
        {
          HWND hcombo = (HWND)lp;
          GetWindowText(hcombo, str, 128);
          std::vector<std::string> _tempList;
          //ComboBox_SelectString(hwndComboBox, 0, L"sa");

          for (size_t i = 0; i < elementList.size(); ++i)
          {
            // Delete all items
            std::wstring stemp = std::wstring(elementList[i].begin(), elementList[i].end());
            LPCWSTR sw = stemp.c_str();
            SendMessageW(hwndComboBox, CB_DELETESTRING, 0, (LPARAM)sw);

            // Add new items to vector
            std::wstring ws(str);
            std::string test(ws.begin(), ws.end());
            size_t pos = findSubStrinInString(elementList[i], test);
            if (string::npos != pos)
            {
              _tempList.push_back(elementList[i]);
            }
          }

          for (size_t i = 0; i < _tempList.size(); ++i)
          {
            std::wstring stemp = std::wstring(_tempList[i].begin(), _tempList[i].end());
            LPCWSTR sw = stemp.c_str();
            SendMessageW(hwndComboBox, CB_ADDSTRING, 0, (LPARAM)sw);
          }
          ComboBox_ShowDropdown(hwndComboBox, TRUE);
          //ComboBox_SetCurSel(hwndComboBox, 1);
        }
      }
    } break;
    case WM_CTLCOLORMSGBOX:
    case WM_CTLCOLORLISTBOX:
    case WM_CTLCOLORBTN:
    case WM_CTLCOLORDLG:
    case WM_CTLCOLORSCROLLBAR:
    case WM_CTLCOLORSTATIC:
    case WM_CTLCOLOREDIT:
    {
      HDC hdcStatic = (HDC)wp;
      SetTextColor(hdcStatic, RGB(240, 216, 192));
      SetBkColor(hdcStatic, RGB(79, 78, 77));
      return (INT_PTR)CreateSolidBrush(RGB(107, 105, 99));
    } break;
    case WM_CLOSE:
    {
      appIsRunning = FALSE;
      PostQuitMessage(0);
      return 0;
    } break;
    default:
    {
      return DefWindowProcW(hWnd, msg, wp, lp);
    } break;
  }
  return DefWindowProc(hWnd, msg, wp, lp);
}

void parseCommandLine()
{
  int argc = __argc;
  char** argv = __argv;
  for (size_t i = 0; i < argc; ++i)
  {
    if (0 == strcmp(argv[i], "-elements"))
    {
      hasElements = true;
      elements = argv[i + 1];

      string item;
      stringstream ss (elements);
      while (getline (ss, item, ';'))
      {
        elementList.push_back(item);
      }
    }
    else if (0 == strcmp(argv[i], "-prompt"))
    {
      hasPrompt = true;
      prompt = argv[i + 1];
      promptLength = strlen(prompt);
    }
    else if (0 == strcmp(argv[i], "-caseInsensitive"))
    {
      caseSensitive = false;
    }
    else if (0 == strcmp(argv[i], "-b"))
    {
      bottom = true;
    }
    else if (0 == strcmp(argv[i], "-fontName"))
    {
      hasFontName = true;
      fontName = argv[i + 1];
    }
    else if (0 == strcmp(argv[i], "-fontSize"))
    {
      hasFontSize = true;

      int ival;
      std::stringstream ss(argv[i + 1]);
      ss >> ival;

      fontSize = ival;
    }
    else if (0 == strcmp(argv[i], "-lineNumber"))
    {
      hasLineNumber = true;

      int ival;
      std::stringstream ss(argv[i + 1]);
      ss >> ival;

      lineNumber = ival;
    }
    else if (0 == strcmp(argv[i], "-version"))
    {
      printf("version: %s\n", version.c_str());
      SendMessage(hwnd, WM_CLOSE, 0, 0);
    }
    else
    {
    }
  }
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
  INT Result = 0;
  if (!appAlreadyRunning())
  {
    parseCommandLine();
    initialize();

    MSG msg;
    WNDCLASS wndclass;

    wndclass.cbClsExtra = 0;
    wndclass.cbWndExtra = 0;
    wndclass.lpszMenuName = NULL;
    wndclass.hInstance = hInstance;
    wndclass.lpfnWndProc = WndProc;
    wndclass.lpszClassName = wAppName;
    wndclass.style = CS_HREDRAW | CS_VREDRAW | CS_NOCLOSE;
    wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
    wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);

    if (!RegisterClass(&wndclass))
    {
      MessageBox(NULL, TEXT("This program requires Windows NT!"), wAppName, MB_ICONERROR);
      return 0;
    }
    hwnd = CreateWindow(wAppName,
      TEXT("wmenu"),
      WS_POPUP|WS_VISIBLE|WS_SYSMENU,
      0,                                 // initial x position
      0,                                 // initial y position
      width,                             // initial width
      hasElements ? height/5 : 20,       // initial height
      NULL,                              // parent window handle
      NULL,                              // window menu handle
      hInstance,                         // program instance handle
      NULL);                             // creation parameters

    ShowWindow(hwnd, iCmdShow);
    UpdateWindow(hwnd);

    appIsRunning = TRUE;
    while (GetMessage(&msg, NULL, 0, 0))
    {
      TranslateMessage(&msg);
      DispatchMessage(&msg);

      switch (msg.message)
      {
      case WM_KEYDOWN:
      {
        if ((GetAsyncKeyState(VK_ESCAPE) & 0x01) && appIsRunning)
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
