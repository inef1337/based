#pragma once
#include <cstdint>

namespace based::hooks
{
	bool run_script_threads(std::uint32_t ops_to_execute);

    constexpr int swapchain_num_funcs = 19;
	LRESULT wnd_proc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
	constexpr int present_index = 8;
	HRESULT present(IDXGISwapChain* dis, UINT syncInterval, UINT flags);
	constexpr int resize_buffer_index = 13;
	HRESULT resize_buffer(IDXGISwapChain* dis, UINT bufferCount, UINT width, UINT height, DXGI_FORMAT newFormat, UINT swapChainFlags);
}