#include "gui.hpp"

#include <sstream>
#include <iostream>

#include "../util/util.hpp"
#include "../commandLine/commandLine.hpp"
#include "../commandLine/options/prompt/prompt.hpp"

std::recursive_mutex Gui::m_mutex;
std::atomic<Gui*> Gui::m_gui = nullptr;

Gui::~Gui() {}

Gui* Gui::instance()
{
	auto object = m_gui.load(std::memory_order_relaxed);
	atomic_thread_fence(std::memory_order_acquire);
	if (!object)
	{
		std::lock_guard<std::recursive_mutex> lock(m_mutex);
		object = m_gui.load(std::memory_order_relaxed);
		if (!object)
		{
			object = new Gui;
			std::atomic_thread_fence(std::memory_order_release);
			m_gui.store(object, std::memory_order_relaxed);
		}
	}
	return object;
}

bool Gui::isRunningCheck() {
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

void Gui::setupGui(HWND m_hwnd)
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

      ComboBox::instance()->createComboBox(m_hwnd,
        Prompt::instance()->exists ? Prompt::instance()->length * 8 : 0,
        0,
        width - (Prompt::instance()->length * 8),
        ComboBox::instance()->lineNumber.exists ? ComboBox::instance()->setLineNumber() : 125,
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
      label.createLabel(hwnd,
        0,
        0,
        (Prompt::instance()->length * 8),
        125,
        Font::instance()->name.value,
        Prompt::instance()->value);

      textBox.createTextBox(hwnd,
        Prompt::instance()->exists ? Prompt::instance()->length * 8 : 0,
        0,
        Prompt::instance()->exists ? width - Prompt::instance()->length : width,
        200);
    }
    else
    {
      textBox.createTextBox(hwnd,
        Prompt::instance()->exists ? Prompt::instance()->length * 8 : 0,
        0,
        Prompt::instance()->exists ? width - Prompt::instance()->length : width,
        200);
    }
  }
}

LRESULT CALLBACK WindowHandler(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, Gui& gui)
{
  HDC hdc;
  PAINTSTRUCT ps;
  switch (message)
  {
    case WM_CREATE:
    {
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

            if (!ComboBox::instance()->caseSensitive.exists)
            {
              size_t pos = Util::findSubStrinInString(ComboBox::instance()->items.listOfItems[i], test);
              if (std::string::npos != pos)
              {
                _tempList.push_back(ComboBox::instance()->items.listOfItems[i]);
              }
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
    case WM_CTLCOLORSTATIC:
    case WM_CTLCOLOREDIT:
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

int Gui::initialize(const HINSTANCE hInstance, int iCmdShow)
{
  name = L"wmenu";
  version = "1.0.0";
  width = GetSystemMetrics(SM_CXSCREEN);
  height = GetSystemMetrics(SM_CYSCREEN);

  INT Result = 0;
  if (!isRunningCheck())
  {
    CommandLine commandLine;
    commandLine.parseArguments();

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
      WS_POPUP | WS_VISIBLE | WS_SYSMENU,
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
