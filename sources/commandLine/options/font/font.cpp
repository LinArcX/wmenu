#include "font.hpp"

std::recursive_mutex Font::m_mutex;
std::atomic<Font*> Font::m_font = nullptr;

Font::~Font() {}

Font* Font::instance()
{
	auto object = m_font.load(std::memory_order_relaxed);
	atomic_thread_fence(std::memory_order_acquire);
	if (!object)
	{
		std::lock_guard<std::recursive_mutex> lock(m_mutex);
		object = m_font.load(std::memory_order_relaxed);
		if (!object)
		{
			object = new Font;
			std::atomic_thread_fence(std::memory_order_release);
			m_font.store(object, std::memory_order_relaxed);
		}
	}
	return object;
}
