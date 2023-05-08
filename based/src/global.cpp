#include "global.hpp"
#include "util/functions.hpp"
#include "../dependencies/minhook/minhook.h"

#include <format> 
#include "hooking/pointers.hpp"
#include "hooking/hooking.hpp"
#include "hooking/d3d/renderer.hpp"

namespace based::global
{
	bool alive = true;

	void init_logger() {
		console = std::make_unique<logger>();
		if (!console) {
			MessageBoxA(nullptr, "failed to create logger", 0, 0);
			alive = false;
		}
		console->log_to_console(log_color::green, "info", "initialized logger");
	}

	void init_minhook() {
		const auto status = MH_Initialize();
		if (status != MH_OK) {
			throw std::runtime_error(std::format("failed to initialize MinHook - error code: {}", static_cast<int>(status)));
		}
		console->log_to_console(log_color::green, "info", "initialized minhook");
	}

	void uninit_minhook() {
		const auto status = MH_Uninitialize();
		if (status != MH_OK) {
			throw std::runtime_error(std::format("failed to uninitialize MinHook - error code: {}", static_cast<int>(status)));
		}
		console->log_to_console(log_color::green, "info", "Uninitialized MinHook");
	}

	void init_pointers() {
		ptr = std::make_unique<pointers>();
		if (!ptr) {
			throw std::runtime_error("failed to initialize patterns");
		}
		console->log_to_console(log_color::green, "info", "initialized pointers");
	}

	void init_renderer() {
		render = std::make_unique<renderer>();
		if (!render) {
			throw std::runtime_error("failed to initialize renderer");
		}
		console->log_to_console(log_color::green, "info", "initialized renderer");
	}

	void init_submenu_handler() {
		submenu_handler = std::make_unique<gui::submenu_handler>();
		if (!submenu_handler) {
			throw std::runtime_error("failed to initialize submenu handler");
		}
		console->log_to_console(log_color::green, "info", "initialized submenu handler");
	}

	void init_control_interface() {
		control = std::make_unique<gui::control_interface>();
		if (!control) {
			throw std::runtime_error("failed to initialize control interface");
		}
		console->log_to_console(log_color::green, "info", "initialized control interface");
	}

	void init_hooking() {
		hooking_mgr = std::make_unique<hooking>();
		if (!hooking_mgr) {
			throw std::runtime_error("failed to initialize hooking manager");
		}
		console->log_to_console(log_color::green, "info", "initialized hooking manager");
	}

	void get_input() {
		if (util::is_key_pressed(VK_DELETE) && util::is_key_pressed(VK_END)) {
			alive = false;
		}
	}
}