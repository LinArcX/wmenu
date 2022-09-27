#include "textBox.hpp"

#include <stdio.h>
#include <windowsx.h>
#include <commctrl.h>

TextBox::TextBox()
{
  m_hwnd_id = 4;
  m_hwnd = NULL;
  m_window_proc = NULL;
}

LRESULT CALLBACK TextBoxWindowHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam, TextBox& textBox)
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
  return CallWindowProc(textBox.m_window_proc, hwnd, msg, wParam, lParam);
}

void TextBox::createTextBox(HWND hwnd, int x, int y, int width, int height)
{
   hwnd = CreateWindow(WC_EDIT, L"",
     WS_BORDER | WS_CHILD | WS_VISIBLE | WS_VISIBLE | ES_WANTRETURN,
     x, y, width, height,
     hwnd, (HMENU)hwndId(), NULL, NULL);

    m_window_proc = (WNDPROC) SetWindowLongPtr(hwnd, GWLP_WNDPROC, (LONG_PTR)TextBoxWindowHandler);
    SetFocus(hwnd);
}

