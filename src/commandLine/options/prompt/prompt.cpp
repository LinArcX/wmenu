#include "prompt.hpp"

std::recursive_mutex Prompt::m_mutex;
std::atomic<Prompt*> Prompt::m_prompt = nullptr;

Prompt::~Prompt() {}

Prompt* Prompt::instance()
{
	auto object = m_prompt.load(std::memory_order_relaxed);
	atomic_thread_fence(std::memory_order_acquire);
	if (!object)
	{
		std::lock_guard<std::recursive_mutex> lock(m_mutex);
		object = m_prompt.load(std::memory_order_relaxed);
		if (!object)
		{
			object = new Prompt;
			std::atomic_thread_fence(std::memory_order_release);
			m_prompt.store(object, std::memory_order_relaxed);
		}
	}
	return object;
}
