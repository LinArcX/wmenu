#include <stdio.h>
#include <windows.h>
#include <windowsx.h>
#include <commctrl.h>

#include <list>
#include <vector>
#include <sstream>
#include <iostream>

using namespace std;

#define WCOMBO 3
#define APP_NAME "wmenu"
#pragma warning(disable: 4996)

HWND hwnd;
BOOL appIsRunning;
HWND hwndComboBox = NULL;
std::vector<std::string> elementList;
static TCHAR wAppName[] = TEXT(APP_NAME);

int width, height;
bool bottom = false;
std::list<int> items;
char* elements = NULL;

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

void createComboBox(HWND hwnd)
{
  hwndComboBox = CreateWindow(WC_COMBOBOX, L"",
    CBS_DROPDOWN | CBS_HASSTRINGS | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE,
    0, 0, width, 200, hwnd, (HMENU)WCOMBO, NULL, NULL);

  //HFONT hFont = CreateFont(20, 8, 0, 0,
  //  FW_BOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS,
  //  CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, NULL, TEXT("Vazir"));
  //SendMessage(hwndComboBox, WM_SETFONT, (WPARAM)hFont, 0); // GetStockObject(ANSI_FIXED_FONT)

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

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{
  HDC hdc;
  PAINTSTRUCT ps;
  switch (msg)
  {
    case WM_CREATE:
    {
      createComboBox(hWnd);
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
          //MessageBoxW(hWnd, buf, L"Good Example", 0);
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

          for (size_t i = 0; i < elementList.size(); ++i)
          {
            // Delete all items
            std::wstring stemp = std::wstring(elementList[i].begin(), elementList[i].end());
            LPCWSTR sw = stemp.c_str();
            SendMessageW(hwndComboBox, CB_DELETESTRING, 0, (LPARAM)sw);

            // Add new items to vector
            std::wstring ws(str);
            std::string test(ws.begin(), ws.end());
            if (elementList[i].find(test) != string::npos)
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

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
  int argc = __argc;
  char** argv = __argv;
  INT Result = 0;
  bool hasElements = false;
  appIsRunning = TRUE;

  if (appAlreadyRunning())
  {
    goto Exit;
  }

  for (size_t i = 0; i < argc; ++i)
  {
    if (0 == strcmp(argv[i], "-e") || 0 == strcmp(argv[i], "--elements"))
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
    else if (strcmp(argv[i],"-b") == 0)
    {
      bottom = true;
    }
    else if (strcmp(argv[i],"-v") == 0)
    {
      printf("wmenu version 0.1.0\n");
      return 0;
    }
    else if (strcmp(argv[i],"-p")==0)
    {
    }
    else if (strcmp(argv[i],"-f")==0)
    {
    }
  }
  if (!hasElements)
  {
    goto Exit;
  }

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
    0,              // initial x position
    0,              // initial y position
    width,          // initial width
    height/5,       // initial height
    NULL,           // parent window handle
    NULL,           // window menu handle
    hInstance,      // program instance handle
    NULL);          // creation parameters

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
      if ((GetAsyncKeyState(VK_ESCAPE) & 0x01) && appIsRunning)
        {
          SendMessage(hwnd, WM_CLOSE, 0, 0);
        }
    } break;
    }
  }
  Result = msg.wParam;

Exit:
  return Result;
}
