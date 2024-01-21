#include "util.hpp"

#include <sstream>
#include <algorithm>

#pragma warning(disable: 4996)

LPWSTR Util::charPtrToLpcwstr(const char* source)
{
  wchar_t wtext[500];
  mbstowcs(wtext, source, strlen(source) + 1);
  LPWSTR ptr = wtext;
  return ptr;
}

std::wstring Util::stringToLpcwstr(std::string source)
{
  std::wstring temp = std::wstring(source.begin(), source.end());
  return temp;
}

void Util::setFont(HWND hwnd, char* fontName, int height, int width)
{
    wchar_t wtext[400];
    mbstowcs(wtext, fontName, strlen(fontName) + 1);
    LPWSTR ptr = wtext;

    HFONT hFont = CreateFont(
      height, width,
      0, 0,
      FW_BOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS,
      CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, NULL, ptr);
    SendMessage(hwnd, WM_SETFONT, (WPARAM)hFont, 0);
}

int Util::stringToInt(std::string input)
{
  int output;
  std::stringstream stream(input);
  stream >> output;
  return output;
}

size_t Util::findSubStrinInString(std::string data, std::string subString, size_t pos)
{
  std::transform(data.begin(), data.end(), data.begin(), ::tolower);
  std::transform(subString.begin(), subString.end(), subString.begin(), ::tolower);
  return data.find(subString, pos);
}

std::wstring Util::stringToWString(const std::string& str)
{
  int size_needed = MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), NULL, 0);
  std::wstring wstrTo( size_needed, 0 );
  MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), &wstrTo[0], size_needed);
  return wstrTo;
}

bool Util::isDelimiterValid(char delimiter)
{
  bool isValid = false;
  switch (delimiter)
  {
  case ',':
  case ';':
  case ':':
  case 32: // SPACE
  case '-':
  case '_':
  case '|':
    isValid = true;
    break;
  default:
    isValid = false;
    break;
  }
  return isValid;
}

bool Util::containsCarriageOrTab(char raw)
{
  bool result = false;
  switch (raw)
  {
  case 'n':
  case 'r':
  case 't':
    result = true;
    break;
   default:
     result = false;
    break;
  }
  return result;
}

char Util::convertCarriageOrTabToAscii(char raw)
{
  char result;
  switch (raw)
  {
  case 'n':
    result = 10;
    break;
  case 'r':
    result = 13;
    break;
  case 't':
    result = 9;
    break;
   default:
    result = raw;
    break;
  }
  return result;
}

