#ifndef COMMANDLINE_OPTIONS_FONT_HPP
#define COMMANDLINE_OPTIONS_FONT_HPP

#include <windows.h>
#include <stdbool.h>

typedef struct name
{
	bool exists;
	char* value;
} Name;

typedef struct size
{
	bool exists;
	int value;
} Size;

typedef struct font
{
  Name name;
  Size size;
} Font;

void setFont(HWND hwnd, char* fontName, int height, int width);

#endif


//#include <mutex>
//#include <atomic>
//
//
//class Font
//{
//public:
//	~Font();
//
//	static Font* instance();
//
//	Name name;
//	Size size;
//
//private:
//	static std::recursive_mutex m_mutex;
//	static std::atomic<Font*> m_font;
//};
//
