#pragma once

#include <memory>
#include <string_view>
#include "../util/memory.hpp"

namespace based::global
{
	class detour
	{
	public:
		template <class T>
		detour(const std::string_view name, T address, void* detour);
		~detour();

		std::string_view m_name;
		void** m_address;
		void* m_detour;
		void* m_original;

		void enable_hook();
		void disable_hook();
	};

	class hooking
	{
	public:
		hooking();
		~hooking();

		void create_hooks();
		void free_hooks();

		void enable_hooks();
		void disable_hooks();

		std::unique_ptr<detour> m_run_script_threads;

		vmt_hook m_swapchain_hook;
	};

	inline std::unique_ptr<hooking> hooking_mgr;
}