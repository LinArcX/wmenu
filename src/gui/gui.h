//#ifndef GUI_HPP
//#define GUI_HPP
//
//#include <list>
//#include <vector>
//#include <mutex>
//#include <atomic>
//
//#include "label/label.hpp"
//#include "textBox/textBox.hpp"
//#include "comboBox/comboBox.hpp"
//#include "../commandLine/options/font/font.hpp"
//
//enum class Position
//{
//	Top,
//	Center,
//	Bottom
//};
//
//class Gui
//{
//public:
//	~Gui();
//
//	static Gui* instance();
//
//	int initialize(const HINSTANCE hInstance, int iCmdShow);
//
//	friend LRESULT CALLBACK WindowHandler(HWND, UINT, WPARAM, LPARAM, Gui&);
//
//	bool isRunningCheck();
//
//	void setupGui(HWND hwnd);
//
//	HWND hwnd;
//	int width;
//	int height;
//
//	std::wstring name;
//	std::string version;
//
//	Label label;
//	TextBox textBox;
//	Position position = Position::Top;
//
//private:
//	static std::recursive_mutex m_mutex;
//	static std::atomic<Gui*> m_gui;
//};
//LRESULT CALLBACK WindowHandler(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, Gui& gui);
//
//#endif
