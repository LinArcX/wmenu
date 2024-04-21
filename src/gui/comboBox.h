#ifndef GUI_COMBOBOX_HPP
#define GUI_COMBOBOX_HPP

#include <windows.h>
#include <windowsx.h>
#include <commctrl.h>
#include <stdio.h>
#include <stdbool.h>

typedef struct items
{
	bool exists;
  char* rawItems;
  char** compoundItems;
} Items;

typedef struct position
{
	int x;
	int y;
} Position;

typedef struct dimension
{
  int width;
  int height;
} Dimension;

typedef struct lineNumber
{
	bool exists;
	int value;
} LineNumber;

typedef struct caseSensitive
{
	bool exists;
} CaseSensitive;

typedef struct comboBox
{
	Items items;
	Position position;
  Dimension dimension;
	LineNumber lineNumber;
	CaseSensitive caseSensitive;

	HWND hwnd;
	int hwnd_id ;
} ComboBox;

void createComboBox(ComboBox* comboBox, Font* font, HWND* hwnd);
#endif



//#include <string.h>
//#include <wchar.h>
//#include <stdlib.h>

//#include <stdio.h>
//#include <windows.h>
//#include <windowsx.h>
//#include <commctrl.h>
//
//#include <mutex>
//#include <atomic>
//#include <list>
//#include <vector>
//#include <iostream>
//
//struct Items
//{
//	bool exists = false;
//	std::string rawItems = "";
//	std::vector<std::string> listOfItems;
//};
//
//struct LineNumber
//{
//	bool exists = false;
//	int value = 0;
//};
//
//struct CaseSensitive
//{
//	bool exists = true;
//};
//
////struct Position
////{
////	int x = 0;
////	int y = 0;
////};
//
//class ComboBox
//{
//public:
//	~ComboBox();
//
//	static ComboBox* instance();
//
//	int setLineNumber();
//	void createComboBox(HWND hwnd, int x, int y, int width, int height, char* fontName);
//
//	Items items;
//	HWND hwnd = NULL;
//	int hwnd_id = 3 ;
//	LineNumber lineNumber;
//	CaseSensitive caseSensitive;
//	//Position position;
//
//private:
//	static std::atomic<ComboBox*> m_comboBox;
//	static std::recursive_mutex m_mutex;
//};
//
