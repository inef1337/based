#include "pointers.hpp"
#include "..//util/functions.hpp"
#include "..//util/memory.hpp"
#include <dxgi.h>
#include <d3d11.h>

namespace based::global
{
	pointers::pointers()
		: m_wndproc(0), m_window(0), m_game_state(0), m_run_script_threads(0)
	{
		m_window = reinterpret_cast<HWND>(util::check_ptr("w", FindWindow("grcWindow", "Grand Theft Auto V")));
		m_game_state = reinterpret_cast<std::uint32_t*>(util::check_ptr("gs", signature("48 85 C9 74 4B 83 3D").scan().add(7).rip().as<LPVOID>()));
		m_fix_vectors = reinterpret_cast<types::fix_vectors_t>(util::check_ptr("fv", signature("83 79 18 00 48 8B D1 74 4A FF 4A 18 48 63 4A 18 48 8D 41 04 48 8B 4C CA").scan().as<types::fix_vectors_t>()));
		m_get_native_handler = reinterpret_cast<types::get_native_handler_t>(util::check_ptr("ghn", signature("48 8D 0D ? ? ? ? 48 8B 14 FA E8 ? ? ? ? 48 85 C0 75 0A").scan().add(12).rip().as<types::get_native_handler_t>()));
		m_native_registration_table = reinterpret_cast<rage::scrNativeRegistrationTable*>(util::check_ptr("nrt", signature("48 8D 0D ? ? ? ? 48 8B 14 FA E8 ? ? ? ? 48 85 C0 75 0A").scan().add(3).rip().as<rage::scrNativeRegistrationTable*>()));
		m_swap_chain = reinterpret_cast<IDXGISwapChain**>(util::check_ptr("SC", signature("48 8B 0D ? ? ? ? 48 8B 01 44 8D 43 01 33 D2 FF 50 40 8B C8").scan().add(3).rip().as<IDXGISwapChain**>()));

		m_script_threads = signature("45 33 F6 8B E9 85 C9 B8").scan().sub(4).rip().sub(8).as<decltype(m_script_threads)>();

		m_native_return = util::check_ptr("nr", signature("FF E3").scan().as<LPVOID>());
		m_run_script_threads = util::check_ptr("rst", signature("45 33 F6 8B E9 85 C9 B8").scan().sub(0x1F).as<decltype(m_run_script_threads)>());
	}
}