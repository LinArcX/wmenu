#include <stdio.h>
#include <stddef.h>
#include "comboBox.h"
#include "../commandLine/options/font/font.h"

void 
createComboBox(ComboBox* comboBox, Font* font, HWND* hwnd)
{
  comboBox->hwnd = CreateWindow(WC_COMBOBOX, L"", CBS_DROPDOWN | CBS_HASSTRINGS | WS_CHILD | WS_VISIBLE,
    comboBox->position.x, comboBox->position.y, 
    comboBox->dimension.width, comboBox->dimension.height,
    hwnd, (HMENU)comboBox->hwnd_id, NULL, NULL);

  if (font->name.exists)
  {
    setFont(hwnd, font->name,
      font->size.exists ? (font->size.value * 2) + (font->size.value / 2) : 20,
      font->size.exists ? font->size.value : 8);
  }

  int itemsSize = 2dCharArraySize(comboBox->items.compoundItems);
  if (itemsSize)
  {
    for (size_t i = 0; i < itemsSize; ++i)
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

//, HWND m_hwnd, int x, int y, int width, int height, char* fontName)
//#include "../../util/util.hpp"
//#include "../../commandLine/options/font/font.hpp"
//#include "../../commandLine/options/prompt/prompt.hpp"
//
//std::recursive_mutex ComboBox::m_mutex;
//std::atomic<ComboBox*> ComboBox::m_comboBox = nullptr;
//
//ComboBox::~ComboBox() {}
//
//ComboBox* ComboBox::instance()
//{
//	auto object = m_comboBox.load(std::memory_order_relaxed);
//	atomic_thread_fence(std::memory_order_acquire);
//	if (!object)
//	{
//		std::lock_guard<std::recursive_mutex> lock(m_mutex);
//		object = m_comboBox.load(std::memory_order_relaxed);
//		if (!object)
//		{
//			object = new ComboBox;
//			std::atomic_thread_fence(std::memory_order_release);
//			m_comboBox.store(object, std::memory_order_relaxed);
//		}
//	}
//	return object;
//}
