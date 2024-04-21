#include "font.h"
#include <stddef.h>

void
setFont(HWND hwnd, char* fontName, int height, int width)
{
    wchar_t wtext[400];
    mbstowcs(wtext, fontName, strlen(fontName) + 1);
    LPWSTR ptr = wtext;

    HFONT hFont = CreateFont(height, width, 0, 0,
      FW_BOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS,
      CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, NULL, ptr);
    SendMessage(hwnd, WM_SETFONT, (WPARAM)hFont, 0);
}

//std::recursive_mutex Font::m_mutex;
//std::atomic<Font*> Font::m_font = nullptr;
//
//Font::~Font() {}
//
//Font* Font::instance()
//{
//	auto object = m_font.load(std::memory_order_relaxed);
//	atomic_thread_fence(std::memory_order_acquire);
//	if (!object)
//	{
//		std::lock_guard<std::recursive_mutex> lock(m_mutex);
//		object = m_font.load(std::memory_order_relaxed);
//		if (!object)
//		{
//			object = new Font;
//			std::atomic_thread_fence(std::memory_order_release);
//			m_font.store(object, std::memory_order_relaxed);
//		}
//	}
//	return object;
//}
