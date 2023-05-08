#pragma once

#include <string_view>
#include "logger.hpp"
#include "../../dependencies/minhook/minhook.h"

using namespace based::global; 

//while it's generally considered bad practice to be using namespace directives in header files, an exception is made as this file is relatively small

namespace based::util {
	class detour
	{
	public:
		template <class T>
		detour(const std::string_view name, T address, void* detour)
			:
			m_name(name),
			m_address(reinterpret_cast<void**>(&address)),
			m_detour(detour),
			m_original(reinterpret_cast<void*>(*m_address))
		{
			const auto result = MH_CreateHook(*m_address, m_detour, &m_original);
			if (result != MH_OK) {
				console->log_to_console(log_color::green, "info", "failed to hook {}", static_cast<LPVOID>(*m_address));;
				throw std::runtime_error(std::to_string(result));
			}
			console->log_to_console(log_color::blue | log_color::intensify | log_color::red, "info", "hooked {} at {}", m_name, *m_address);
		}

		~detour() {
			MH_RemoveHook(this->m_address);
			console->log_to_console(log_color::green, "info", "unhooked {}", m_name);
		}

		void enable_hook() {
			MH_EnableHook(this->m_address);
		}

		void disable_hook() {
			MH_DisableHook(this->m_address);
		}

	public:
		std::string_view m_name;
		void** m_address;
		void* m_detour;
		void* m_original;
	};
}