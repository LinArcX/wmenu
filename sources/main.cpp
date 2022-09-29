#include "gui/gui.hpp"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
  return Gui::instance()->initialize(hInstance, iCmdShow);
}
