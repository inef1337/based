#pragma once
#include <iostream>
#include <Windows.h>
#include <cstdint>
#include <vector>
#include <sstream>
#include <fstream>
#include <filesystem>
#include <memory>
#include <future>
#include <dxgi.h>
#include <d3d11.h>
#include "../rage/invoker.hpp"
#include "..//rage/classes/classes.hpp"

namespace based::global
{
	namespace types
	{
		using fix_vectors_t = void(*)(rage::scrNativeCallContext* call_ctx);
		using get_native_handler_t = rage::scrNativeHandler(*)(rage::scrNativeRegistrationTable* registration_table, rage::scrNativeHash hash);
	}

	class pointers
	{
	public:
		pointers();

		WNDPROC m_wndproc{};
		HWND m_window{};
		std::uint32_t* m_game_state;

		types::fix_vectors_t m_fix_vectors{};
		types::get_native_handler_t m_get_native_handler{};
		rage::scrNativeRegistrationTable* m_native_registration_table{};
		IDXGISwapChain** m_swap_chain{};

		PVOID m_native_return{};
		LPVOID m_run_script_threads{};

		gta::rage::atArray<gta::GtaThread*>* m_script_threads{};
	};
	inline std::unique_ptr<pointers> ptr;

	template <typename F, typename ...Args>
	inline void execute_script(std::uint32_t script_hash, F&& callback, Args &&...args)
	{
		auto tls_ctx = gta::rage::tlsContext::get();
		for (auto thread : *ptr->m_script_threads)
		{
			if (!thread || !thread->m_context.m_thread_id || thread->m_context.m_script_hash != script_hash)
				continue;

			auto og_thread = tls_ctx->m_script_thread;

			tls_ctx->m_script_thread = thread;
			tls_ctx->m_is_script_thread_active = true;

			std::invoke(std::forward<F>(callback), std::forward<Args>(args)...);

			tls_ctx->m_script_thread = og_thread;
			tls_ctx->m_is_script_thread_active = og_thread != nullptr;

			return;
		}
	}

	
}