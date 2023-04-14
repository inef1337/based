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
#include "..//hooking.hpp"
#include "../../../dependencies/imgui/imgui.h"
#include <dxgi.h>
#include "../../../dependencies/imgui/imgui_impl_dx11.h"
#include <d3d11.h>
#include <wrl/client.h>

namespace based
{
	template <typename com>
	using comptr = Microsoft::WRL::ComPtr<com>;

	class renderer
	{
	public:
		explicit renderer();
		~renderer();

		void on_present();

		void pre_reset();
		void post_reset();

		void wnd_proc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

		void allocate();

		void text(
			const char* text,
			float pos_x,
			float pos_y,
			ImColor& color,
			ImFont* font
		);

		void rect(
			float pos_x,
			float pos_y,
			float size_x,
			float size_y,
			ImColor& color
		);

		void texture(
			ImTextureID texture,
			float pos_x,
			float pos_y,
			float size_x,
			float size_y,
			ImColor& color
		);

	public:
		ImFont* m_font;

		comptr<IDXGISwapChain> m_dxgi_swapchain;
		comptr<ID3D11Device> m_d3d_device;
		comptr<ID3D11DeviceContext> m_d3d_device_context;
	};

	namespace global
	{
		inline std::unique_ptr<renderer> render;
	}

}


