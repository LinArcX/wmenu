//std::recursive_mutex Gui::m_mutex;
//std::atomic<Gui*> Gui::m_gui = nullptr;
//
//Gui::~Gui() {}
//
//Gui* Gui::instance()
//{
//	auto object = m_gui.load(std::memory_order_relaxed);
//	atomic_thread_fence(std::memory_order_acquire);
//	if (!object)
//	{
//		std::lock_guard<std::recursive_mutex> lock(m_mutex);
//		object = m_gui.load(std::memory_order_relaxed);
//		if (!object)
//		{
//			object = new Gui;
//			std::atomic_thread_fence(std::memory_order_release);
//			m_gui.store(object, std::memory_order_relaxed);
//		}
//	}
//	return object;
//}
