#ifndef GUI_COMBOBOX_HPP
#define GUI_COMBOBOX_HPP

#include <stdio.h>
#include <windows.h>
#include <windowsx.h>
#include <commctrl.h>

#include <mutex>
#include <atomic>
#include <list>
#include <vector>
#include <iostream>

struct Items
{
	bool exists = false;
	std::string rawItems = "";
	std::vector<std::string> listOfItems;
};

struct LineNumber
{
	bool exists = false;
	int value = 0;
};

struct CaseSensitive
{
	bool exists = true;
};

class ComboBox
{
public:
	~ComboBox();

	static ComboBox* instance();

	int setLineNumber();
	void createComboBox(HWND hwnd, int x, int y, int width, int height, char* fontName);

	Items items;
	HWND hwnd = NULL;
	int hwnd_id = 3 ;
	LineNumber lineNumber;
	CaseSensitive caseSensitive ;

private:
	static std::atomic<ComboBox*> m_comboBox;
	static std::recursive_mutex m_mutex;
};

#endif
