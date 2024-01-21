#ifndef UTIL_HPP
#define UTIL_HPP

#include <windows.h>
#include <iostream>

class Util {
public:
	static LPWSTR charPtrToLpcwstr(const char* source);
	static std::wstring stringToWString(const std::string& str);
	static std::wstring stringToLpcwstr(std::string source);
	static void setFont(HWND hwnd, char* fontName, int height, int width);
	static int stringToInt(std::string input);
	static size_t findSubStrinInString(std::string data, std::string subString, size_t pos = 0);
	static bool isDelimiterValid(char delimiter);
	static bool containsCarriageOrTab(char seq);
	static char convertCarriageOrTabToAscii(char raw);
};

#endif
