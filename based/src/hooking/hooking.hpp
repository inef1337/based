#pragma once

#include <memory>
#include <string_view>
#include "../util/memory.hpp"
#include "../../dependencies/minhook/minhook.h"
#include "../global.hpp"
#include "..//util/detour.hpp"

namespace based::global
{
	class hooking
	{
	public:
		hooking();
		~hooking();

		void create_hooks();
		void free_hooks();

		void enable_hooks();
		void disable_hooks();

		std::unique_ptr<util::detour> m_run_script_threads;

		vmt_hook m_swapchain_hook;
	};

	inline std::unique_ptr<hooking> hooking_mgr;
}