#pragma once
#include "util/logger.hpp"
#include "gui/submenu_handler.hpp"
#include "gui/control_interface.hpp"

inline std::string base_name = "based";

namespace based::global
{
	extern bool alive;

	void init_logger();
	void init_minhook();
	void uninit_minhook();
	void init_pointers();
	void init_renderer();
	void init_control_interface();
	void init_submenu_handler();
	void init_hooking();
	void init_queue();
	void get_input();

	inline PVOID vectored_exception_handler{};

	inline std::unique_ptr<gui::submenu_handler> submenu_handler;
	inline std::unique_ptr<gui::control_interface> control;
}