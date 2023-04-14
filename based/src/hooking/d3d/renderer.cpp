#include "../../../dependencies/imgui/imgui.h"
#include "renderer.hpp"
#include "../../../dependencies/imgui/imgui_impl_win32.h"
#include "..//hooking.hpp"
#include "..//pointers.hpp"
#include "..//hooks/hooks.hpp"
#include "..//..//global.hpp"
#include "../../../dependencies/imgui/imgui_internal.h"
#include "..//..//gui/gui.hpp"
#include "..//..//gui/settings.hpp"

IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace based
{
	inline ImVec2 add_vectors(
		ImVec2 vectorA,
		ImVec2 vectorB
	)
	{
		ImVec2 result{};
		result.x = vectorA.x;
		result.y = vectorA.y;
		result.x += vectorB.x;
		result.y += vectorB.y;

		return result;
	}

	renderer::renderer() :
		m_dxgi_swapchain(*global::ptr->m_swap_chain)
	{
		void* d3d_device{};
		if (SUCCEEDED(m_dxgi_swapchain->GetDevice(__uuidof(ID3D11Device), &d3d_device)))
		{
			m_d3d_device.Attach(static_cast<ID3D11Device*>(d3d_device));
		}
		else
		{
			throw std::runtime_error("Failed to get D3D device.");
		}
		m_d3d_device->GetImmediateContext(m_d3d_device_context.GetAddressOf());

		ImGuiContext* ctx = ImGui::CreateContext();

		ImGui_ImplDX11_Init(m_d3d_device.Get(), m_d3d_device_context.Get());
		ImGui_ImplWin32_Init(global::ptr->m_window);

		this->allocate();

		auto& style = ImGui::GetStyle();
		style.WindowPadding = { 10.f, 10.f };
		style.PopupRounding = 0.f;
		style.FramePadding = { 8.f, 4.f };
		style.ItemSpacing = { 10.f, 8.f };
		style.ItemInnerSpacing = { 6.f, 6.f };
		style.TouchExtraPadding = { 0.f, 0.f };
		style.IndentSpacing = 21.f;
		style.ScrollbarSize = 13.f;
		style.GrabMinSize = 4.f;
		style.WindowBorderSize = 0.f;
		style.ChildBorderSize = 0.f;
		style.PopupBorderSize = 0.f;
		style.FrameBorderSize = 0.f;
		style.TabBorderSize = 0.f;
		style.WindowRounding = 0.f;
		style.ChildRounding = 0.f;
		style.FrameRounding = 0.f;
		style.ScrollbarRounding = 4.f;
		style.GrabRounding = 0.f;
		style.TabRounding = 0.f;
		style.WindowTitleAlign = { 0.5f, 0.5f };
		style.ButtonTextAlign = { 0.5f, 0.5f };
		style.DisplaySafeAreaPadding = { 3.f, 3.f };

		auto& colors = style.Colors;
		colors[ImGuiCol_Text] = ImGui::ColorConvertU32ToFloat4(ImColor(255, 255, 255, 255));
		colors[ImGuiCol_TextDisabled] = ImVec4(1.00f, 0.90f, 0.19f, 1.00f);
		colors[ImGuiCol_WindowBg] = ImGui::ColorConvertU32ToFloat4(ImColor(0, 0, 0, 100));
		colors[ImGuiCol_ChildBg] = ImGui::ColorConvertU32ToFloat4(ImColor(50, 30, 30, 0));
		colors[ImGuiCol_PopupBg] = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
		colors[ImGuiCol_Border] = ImGui::ColorConvertU32ToFloat4(ImColor(70, 70, 70, 255));
		colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		colors[ImGuiCol_FrameBg] = ImGui::ColorConvertU32ToFloat4(ImColor(0, 0, 0, 100));
		colors[ImGuiCol_FrameBgHovered] = ImVec4(0.21f, 0.21f, 0.21f, 0.78f);
		colors[ImGuiCol_FrameBgActive] = ImVec4(0.28f, 0.27f, 0.27f, 0.54f);
		colors[ImGuiCol_TitleBg] = ImVec4(0.17f, 0.17f, 0.17f, 1.00f);
		colors[ImGuiCol_TitleBgActive] = ImVec4(0.19f, 0.19f, 0.19f, 1.00f);
		colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
		colors[ImGuiCol_MenuBarBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
		colors[ImGuiCol_ScrollbarBg] = ImVec4(0.02f, 0.02f, 0.02f, 0.0f);
		colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
		colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
		colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
		colors[ImGuiCol_CheckMark] = ImGui::ColorConvertU32ToFloat4(ImColor(150, 150, 150, 255));
		colors[ImGuiCol_SliderGrab] = ImVec4(0.34f, 0.34f, 0.34f, 1.00f);
		colors[ImGuiCol_SliderGrabActive] = ImVec4(0.39f, 0.38f, 0.38f, 1.00f);
		colors[ImGuiCol_Button] = ImGui::ColorConvertU32ToFloat4(ImColor(60, 60, 60, 255));
		colors[ImGuiCol_ButtonHovered] = ImVec4(0.41f, 0.41f, 0.41f, 0.78f);
		colors[ImGuiCol_ButtonActive] = ImVec4(0.41f, 0.41f, 0.41f, 0.87f);
		colors[ImGuiCol_Header] = ImVec4(0.37f, 0.37f, 0.37f, 0.31f);
		colors[ImGuiCol_HeaderHovered] = ImVec4(0.38f, 0.38f, 0.38f, 0.37f);
		colors[ImGuiCol_HeaderActive] = ImVec4(0.37f, 0.37f, 0.37f, 0.51f);
		colors[ImGuiCol_Separator] = ImVec4(0.38f, 0.38f, 0.38f, 0.50f);
		colors[ImGuiCol_SeparatorHovered] = ImVec4(0.46f, 0.46f, 0.46f, 0.50f);
		colors[ImGuiCol_SeparatorActive] = ImVec4(0.46f, 0.46f, 0.46f, 0.64f);
		colors[ImGuiCol_ResizeGrip] = ImVec4(0.26f, 0.26f, 0.26f, 1.00f);
		colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
		colors[ImGuiCol_ResizeGripActive] = ImVec4(0.35f, 0.35f, 0.35f, 1.00f);
		colors[ImGuiCol_Tab] = ImVec4(0.21f, 0.21f, 0.21f, 0.86f);
		colors[ImGuiCol_TabHovered] = ImVec4(0.27f, 0.27f, 0.27f, 0.86f);
		colors[ImGuiCol_TabActive] = ImVec4(0.34f, 0.34f, 0.34f, 0.86f);
		colors[ImGuiCol_TabUnfocused] = ImVec4(0.10f, 0.10f, 0.10f, 0.97f);
		colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.15f, 0.15f, 0.15f, 1.00f);
		colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
		colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
		colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
		colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
		colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
		colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
		colors[ImGuiCol_NavHighlight] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
		colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
		colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
		colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
	}

	renderer::~renderer()
	{
		ImGui_ImplWin32_Shutdown();
		ImGui_ImplDX11_Shutdown();
		ImGui::DestroyContext();
	}

	void renderer::on_present()
	{
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		global::submenu_handler->update();

		ImGui::Render();
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	}

	void renderer::pre_reset()
	{
		ImGui_ImplDX11_InvalidateDeviceObjects();
	}

	void renderer::post_reset()
	{
		ImGui_ImplDX11_CreateDeviceObjects();
	}

	void renderer::wnd_proc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
	{
		ImGui_ImplWin32_WndProcHandler(hwnd, msg, wparam, lparam);
	}

	void renderer::allocate()
	{
		

		gui::settings::header::font = ImGui::GetIO().Fonts->AddFontFromFileTTF("C:\\Fonts\\SignPainter HouseScript Regular.ttf", gui::settings::header::text_size);
		gui::settings::submenubar::font = ImGui::GetIO().Fonts->AddFontFromFileTTF("C:\\Fonts\\ChaletLondonNineteenSixty.ttf", gui::settings::submenubar::text_size);
		gui::settings::option::font = ImGui::GetIO().Fonts->AddFontFromFileTTF("C:\\Fonts\\Roboto-Regular.ttf", gui::settings::option::text_size);
		
		if (gui::settings::footer::image == nullptr)
			D3DX11CreateShaderResourceViewFromMemory(
				m_d3d_device.Get(),
				&based::gui::settings::aob_footer_arrows,
				sizeof(based::gui::settings::aob_footer_arrows),
				&gui::settings::footer::image_info,
				gui::settings::footer::thread_pump,
				&gui::settings::footer::image,
				0
		);

		if (gui::settings::option::ticked_toggle_image == nullptr)
			D3DX11CreateShaderResourceViewFromMemory(
				m_d3d_device.Get(),
				&based::gui::settings::aob_ticked_toggle,
				sizeof(based::gui::settings::aob_ticked_toggle),
				&gui::settings::option::ticked_toggle_image_info,
				gui::settings::option::ticked_toggle_thread_pump,
				&gui::settings::option::ticked_toggle_image,
				0
			);

		if (gui::settings::option::toggle_image == nullptr)
			D3DX11CreateShaderResourceViewFromMemory(
				m_d3d_device.Get(),
				&based::gui::settings::aob_toggle,
				sizeof(based::gui::settings::aob_toggle),
				&gui::settings::option::toggle_image_info,
				gui::settings::option::toggle_thread_pump,
				&gui::settings::option::toggle_image,
				0
		);

		if (gui::settings::option::arrow_image == nullptr)
			D3DX11CreateShaderResourceViewFromMemory(
				m_d3d_device.Get(),
				&based::gui::settings::aob_submenu_arrow,
				sizeof(based::gui::settings::aob_submenu_arrow),
				&gui::settings::option::arrow_image_info,
				gui::settings::option::arrow_thread_pump,
				&gui::settings::option::arrow_image,
				0
		);
	}

	void renderer::text(
		const char* text,
		float pos_x,
		float pos_y,
		ImColor& color,
		ImFont* font
	)
	{
		ImGui::PushFont(font);
		ImGui::GetCurrentWindow()->DrawList->AddText(
			ImVec2(pos_x, pos_y),
			color,
			text
		);
		ImGui::PopFont();
	}

	void renderer::rect(
		float pos_x,
		float pos_y,
		float size_x, float size_y, ImColor& color)
	{
		ImRect rect(
			ImVec2(pos_x, pos_y),
			add_vectors(ImVec2(pos_x, pos_y),
				ImGui::CalcItemSize(ImVec2(size_x, size_y),
					0.0f,
					0.0f
				)
			)
		);

		ImGui::GetCurrentWindow()->DrawList->AddRectFilled(rect.Max, rect.Min, color);
	}

	void renderer::texture(
		ImTextureID texture,
		float pos_x,
		float pos_y,
		float size_x,
		float size_y,
		ImColor& color
	)
	{
		const ImVec2 pos = ImGui::GetCursorScreenPos();
		ImGui::SetCursorPos(ImVec2(pos_x, pos_y));

		ImGui::Image(
			texture,
			ImVec2(size_x, size_y),
			ImVec2(0, 0),
			ImVec2(1, 1),
			color
		);

		ImGui::SetCursorPos(pos);
	}

	LRESULT hooks::wnd_proc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
	{
		if (global::alive)
		{
			global::render->wnd_proc(hwnd, msg, wparam, lparam);
		}

		return CallWindowProcW(global::ptr->m_wndproc, hwnd, msg, wparam, lparam);
	}

	HRESULT hooks::present(IDXGISwapChain* dis, UINT syncInterval, UINT flags)
	{
		if (global::alive)
		{
			global::render->on_present();
		}

		return global::hooking_mgr->m_swapchain_hook.get_original<decltype(&present)>(present_index)(dis, syncInterval, flags);
	}

	HRESULT hooks::resize_buffer(IDXGISwapChain* dis, UINT bufferCount, UINT width, UINT height, DXGI_FORMAT newFormat, UINT swapChainFlags)
	{
		if (global::alive)
		{
			global::render->pre_reset();
			auto hr = global::hooking_mgr->m_swapchain_hook.get_original<decltype(&resize_buffer)>(resize_buffer_index)(dis, bufferCount, width, height, newFormat, swapChainFlags);
			if (SUCCEEDED(hr))
			{
				global::render->post_reset();
			}

			return hr;
		}

		return global::hooking_mgr->m_swapchain_hook.get_original<decltype(&resize_buffer)>(resize_buffer_index)(dis, bufferCount, width, height, newFormat, swapChainFlags);
	}
}


