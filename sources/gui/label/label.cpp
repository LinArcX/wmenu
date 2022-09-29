#include "label.hpp"
#include "../../util/util.hpp"
#include "../../commandLine/options/font/font.hpp"

void Label::createLabel(HWND hwnd, int x, int y, int width, int height, char* fontName, std::wstring text)
{
  HWND hwndPrompt = CreateWindow(L"static", NULL,
    WS_CHILD | WS_VISIBLE,
    x, y, width, height,
    hwnd, NULL,
    (HINSTANCE) GetWindowLong (hwnd, GWL_HINSTANCE), NULL);

  if (Font::instance()->name.exists)
  {
    Util::setFont(hwndPrompt, fontName,
      Font::instance()->size.exists ? (Font::instance()->size.value * 2) + (Font::instance()->size.value / 2) : 20,
      Font::instance()->size.exists ? Font::instance()->size.value : 8);
  }

  SetWindowText(hwndPrompt, text.c_str());
}
