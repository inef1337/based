#include "submenu_handler.hpp"
#include "..//menu/main_menu.hpp"
#include "control_interface.hpp"
#include "..//global.hpp"
#include "..//util/functions.hpp"

namespace based::gui
{
	void submenu_handler::load() {
		m_main_submenu = main_menu::get();
		m_current_submenu = main_menu::get();
	}

	void submenu_handler::update() {
		if (m_current_submenu) {
			m_current_submenu->update_menu();
		}

		if (m_next_submenu) {
			m_current_submenu = m_next_submenu;
			m_next_submenu = nullptr;
			global::control->set_option_selected(0);
		}
	}

	void submenu_handler::add_submenu(submenu* submenu) {
		m_submenus.push_back(submenu);
	}

	void submenu_handler::set_submenu(submenu* submenu) {
		if (!util::is_valid_ptr(submenu) && !util::is_valid_ptr(m_current_submenu))
			return;

		m_previous_submenu = m_current_submenu;

		m_next_submenu = submenu;
	}

	void submenu_handler::set_submenu_previous() {
		if (m_current_submenu == m_main_submenu) {
			global::control->set_open(false);
			return;
		}

		m_previous_submenu = m_current_submenu;

		if (!util::is_valid_ptr(m_current_submenu->get_submenu_parent())) {
			m_next_submenu = m_main_submenu;
			global::control->set_open(false);
		}
		else {
			m_next_submenu = m_current_submenu->get_submenu_parent();
		}
	}
}