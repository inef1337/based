#pragma once

#include <mutex>
#include <Windows.h>
#include <vector>
#include "script/script.hpp"

namespace based {
	using script_list = std::vector<std::unique_ptr<script>>;

	class script_mgr
	{
	public:
		explicit script_mgr() = default;
		~script_mgr() = default;

		void add_script(std::unique_ptr<script> script)
		{
			std::lock_guard lock(m_mutex);
			m_scripts.push_back(std::move(script));
		}

		void remove_all_scripts()
		{
			std::lock_guard lock(m_mutex);
			m_scripts.clear();
		}

		script_list& scripts()
		{
			return m_scripts;
		}

		void tick()
		{
			static bool ensure_main_fiber = (ConvertThreadToFiber(nullptr), true);
			std::lock_guard lock(m_mutex);
			for (const auto& script : m_scripts) {
				if (script->is_enabled()) {
					script->tick();
				}
			}
		}

	private:
		std::recursive_mutex m_mutex;
		script_list m_scripts;
	};

	inline script_mgr g_script_mgr;
}