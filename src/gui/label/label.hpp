#ifndef GUI_LABEL_HPP
#define GUI_LABEL_HPP

#include <windows.h>
#include <iostream>

struct Label
{
	void createLabel(HWND hwnd, int x, int y, int width, int height, char* fontName, std::wstring text);
};

#endif
