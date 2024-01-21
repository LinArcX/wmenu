#ifndef COMMANDLINE_OPTIONS_FONT_HPP
#define COMMANDLINE_OPTIONS_FONT_HPP

#include <mutex>
#include <atomic>
#include <windows.h>

struct Name
{
	bool exists = false;
	char* value = NULL;
};

struct Size
{
	bool exists = false;
	int value = 0;
};

class Font
{
public:
	~Font();

	static Font* instance();

	Name name;
	Size size;

private:
	static std::recursive_mutex m_mutex;
	static std::atomic<Font*> m_font;
};

#endif
