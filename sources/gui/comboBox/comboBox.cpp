#include "comboBox.hpp"
#include "../../util/util.hpp"
#include "../../commandLine/options/font/font.hpp"
#include "../../commandLine/options/prompt/prompt.hpp"

std::recursive_mutex ComboBox::m_mutex;
std::atomic<ComboBox*> ComboBox::m_comboBox = nullptr;

ComboBox::~ComboBox() {}

ComboBox* ComboBox::instance()
{
	auto object = m_comboBox.load(std::memory_order_relaxed);
	atomic_thread_fence(std::memory_order_acquire);
	if (!object)
	{
		std::lock_guard<std::recursive_mutex> lock(m_mutex);
		object = m_comboBox.load(std::memory_order_relaxed);
		if (!object)
		{
			object = new ComboBox;
			std::atomic_thread_fence(std::memory_order_release);
			m_comboBox.store(object, std::memory_order_relaxed);
		}
	}
	return object;
}

void ComboBox::createComboBox(HWND m_hwnd, int x, int y, int width, int height, char* fontName)
{
  hwnd = CreateWindow(WC_COMBOBOX, L"",
    CBS_DROPDOWN | CBS_HASSTRINGS | WS_CHILD | WS_VISIBLE,
    x, y, width, height,
    m_hwnd, (HMENU)hwnd_id, NULL, NULL);

  if (Font::instance()->name.exists)
  {
    Util::setFont(hwnd,
      fontName,
      Font::instance()->size.exists ? (Font::instance()->size.value * 2) + (Font::instance()->size.value / 2) : 20,
      Font::instance()->size.exists ? Font::instance()->size.value : 8);
  }

  if (items.listOfItems.size())
  {
    for (size_t i = 0; i < items.listOfItems.size(); ++i)
    {
      std::wstring stemp = std::wstring(items.listOfItems[i].begin(), items.listOfItems[i].end());
      LPCWSTR sw = stemp.c_str();
      SendMessageW(hwnd, CB_ADDSTRING, 0, (LPARAM)sw);
    }
    ComboBox_ShowDropdown(hwnd, TRUE);

    RECT rect;
    int comboWidth = 0;
    int comboHeight = 0;

    if(GetWindowRect(hwnd, &rect))
    {
      comboWidth = rect.right - rect.left;
      comboHeight = rect.bottom - rect.top;
    }
    SetWindowPos(m_hwnd, NULL,
      0, 0,
      Prompt::instance()->exists ? width + Prompt::instance()->length * 8 : width,
      comboHeight,
      NULL);
    SetFocus(hwnd);
  }
  else
  {
    printf("There's no elmenets!");
  }
}

int ComboBox::setLineNumber()
{
  int output = 0;
  if (1 == lineNumber.value) { output =  50; }
  else if (2 == lineNumber.value) { output =  85; }
  else if (3 == lineNumber.value) { output = 105; }
  else { output = 105 + ((lineNumber.value - 3) * 20); }
  return output;
}
