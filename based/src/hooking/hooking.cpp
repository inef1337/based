#include "hooking.hpp"
#include "pointers.hpp"
#include "hooks/hooks.hpp"
#include "../../dependencies/minhook/minhook.h"
#include "../global.hpp"
#include "..//util/memory.hpp"
#include "..//util/functions.hpp"

void* test = nullptr;

namespace based::global
{
	template <class T>
	detour::detour(const std::string_view name, T address, void* detour)
		: 
		m_name(name),
		m_address(reinterpret_cast<void**>(&address)),
		m_detour(detour),
		m_original(reinterpret_cast<void*>(*m_address))
	{
		const auto result = MH_CreateHook(*m_address, m_detour, &m_original);
		if (result != MH_OK) {
			console->log_to_console(log_color::green, "info", "hooked {}", (LPVOID)m_address);;
			throw std::runtime_error(std::to_string(result));
		}
		console->log_to_console(log_color::blue | log_color::intensify | log_color::red, "info", "hooked {} at {}", m_name, *m_address);
	}

	detour::~detour() {
		MH_RemoveHook(this->m_address);
		console->log_to_console(log_color::green, "info", "unhooked {}", m_name);
	}

	void detour::enable_hook() {
		MH_EnableHook(this->m_address);
	}

	void detour::disable_hook() {
		MH_DisableHook(this->m_address);
	}

	hooking::hooking()
		: m_swapchain_hook(*ptr->m_swap_chain, hooks::swapchain_num_funcs)
	{
		init_minhook();
		
		m_swapchain_hook.hook(hooks::present_index, &hooks::present);
		m_swapchain_hook.hook(hooks::resize_buffer_index, &hooks::resize_buffer);

		m_swapchain_hook.enable();
		ptr->m_wndproc = reinterpret_cast<WNDPROC>(util::check_ptr("wp", reinterpret_cast<WNDPROC>(SetWindowLongPtrW(ptr->m_window, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&hooks::wnd_proc)))));

	}

	hooking::~hooking() {
		uninit_minhook();
	}

	void hooking::create_hooks() {
		if (hooking_mgr) {
			m_run_script_threads = std::make_unique<detour>("RST", global::ptr->m_run_script_threads, hooks::run_script_threads);
		}
	}

	void hooking::free_hooks() {
		if (hooking_mgr) {
			SetWindowLongPtrW(ptr->m_window, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(ptr->m_wndproc));
			m_swapchain_hook.disable();

			MH_RemoveHook(MH_ALL_HOOKS);
		}
	}

	void hooking::enable_hooks() {
		if (hooking_mgr) {
			MH_EnableHook(MH_ALL_HOOKS);
		}
	}

	void hooking::disable_hooks() {
		if (hooking_mgr) {
			MH_DisableHook(MH_ALL_HOOKS);
		}
	}

	
}
