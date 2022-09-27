#ifndef GUI_TEXTBOX_HPP
#define GUI_TEXTBOX_HPP

#include <Windows.h>

class TextBox
{
public:
	TextBox();
	HWND hwnd() { return m_hwnd; }
	int hwndId() { return m_hwnd_id; }
	WNDPROC windowProc() { return m_window_proc; }
	void setWindowProc(WNDPROC windowProc) { m_window_proc = windowProc; }

	void createTextBox(HWND hwnd, int x, int y, int width, int height);
	friend LRESULT CALLBACK TextBoxWindowHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam, TextBox&);

private:
	HWND m_hwnd;
	int m_hwnd_id;
	WNDPROC m_window_proc;
};
LRESULT CALLBACK TextBoxWindowHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam, TextBox&);

#endif
