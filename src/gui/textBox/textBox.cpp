#include "textBox.hpp"

#include <stdio.h>
#include <windowsx.h>
#include <commctrl.h>

#include "../../util/util.hpp"
#include "../../commandLine/options/font/font.hpp"

HWND TextBox::hwnd = NULL;
int TextBox::hwnd_id = 4;
WNDPROC TextBox::window_proc = NULL;

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

void TextBox::createTextBox(HWND hwnd, int x, int y, int width, int height, char* fontName)
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

