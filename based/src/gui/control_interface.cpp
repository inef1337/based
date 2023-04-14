#include "control_interface.hpp"
#include "..//global.hpp"
#include <Windows.h>
#include <chrono>
#include <mutex>

namespace based::gui
{
	bool control_interface::is_open_key_pressed() const {
		return m_open_key_pressed;
	}

	bool control_interface::is_back_key_pressed() const {
		return m_back_key_pressed;
	}

	bool control_interface::is_enter_key_pressed() const {
		return m_enter_key_pressed;
	}

	bool control_interface::is_up_key_pressed() const {
		return m_up_key_pressed;
	}

	bool control_interface::is_down_key_pressed() const {
		return m_down_key_pressed;
	}

	bool control_interface::is_left_key_pressed() const {
		return m_left_key_pressed;
	}

	bool control_interface::is_right_key_pressed() const {
		return m_right_key_pressed;
	}

	bool control_interface::is_option_selected(int index) {
		if (index == m_selected_option)
			return true;

		return false;
	}

	bool control_interface::is_open() {
		return m_open;
	}

	bool control_interface::is_option_pressed(const int index) {
		static std::chrono::time_point<std::chrono::steady_clock> last_clicked;
		constexpr auto delay = std::chrono::milliseconds(200);
		auto now = std::chrono::steady_clock::now();

		if (now - last_clicked >= delay) {
			if (is_enter_key_pressed() && is_option_selected(index)) {
				last_clicked = now;
				return true;
			}
		}

		return false;
	}

	bool control_interface::is_option_hovered(const int index) {
		if (is_option_selected(index))
			return true;

		return false;
	}

	bool control_interface::is_key_pressed(std::uint16_t key) {
		if (GetForegroundWindow() == FindWindowA("grcWindow", "Grand Theft Auto V")) {
			if (GetAsyncKeyState(key) & 0x8000) {
				return true;
			}
		}
		return false;
	}

	void control_interface::get_input() {
		reset_input();

		m_open_key_pressed = is_key_pressed(VK_INSERT);
		m_back_key_pressed = is_key_pressed(VK_BACK);
		m_enter_key_pressed = is_key_pressed(VK_RETURN);
		m_up_key_pressed = is_key_pressed(VK_UP);
		m_down_key_pressed = is_key_pressed(VK_DOWN);
		m_left_key_pressed = is_key_pressed(VK_LEFT);
		m_right_key_pressed = is_key_pressed(VK_RIGHT);
	}

	void control_interface::handle_input() {
		static std::chrono::time_point<std::chrono::steady_clock> last_open_key_press;
		static std::chrono::time_point<std::chrono::steady_clock> last_back_key_press;
		static std::chrono::time_point<std::chrono::steady_clock> last_enter_key_press;
		static std::chrono::time_point<std::chrono::steady_clock> last_up_key_press;
		static std::chrono::time_point<std::chrono::steady_clock> last_down_key_press;
		static std::chrono::time_point<std::chrono::steady_clock> last_left_key_press;
		static std::chrono::time_point<std::chrono::steady_clock> last_right_key_press;

		constexpr auto delay = std::chrono::milliseconds(100);
		auto now = std::chrono::steady_clock::now();

		if (m_open_key_pressed && (now - last_open_key_press) >= delay) {
			last_open_key_press = now;
			m_open = !m_open;
		}

		if (m_up_key_pressed && (now - last_up_key_press) >= delay) {
			last_up_key_press = now;
			this->set_option_selected(m_selected_option - 1);
		}

		if (m_down_key_pressed && (now - last_down_key_press) >= delay) {
			last_down_key_press = now;
			this->set_option_selected(m_selected_option + 1);
		}

		if (m_back_key_pressed && (now - last_back_key_press) >= delay) {
			last_back_key_press = now;
			global::submenu_handler->set_submenu_previous();
		}

		if (m_enter_key_pressed && (now - last_enter_key_press) >= delay) {
			last_enter_key_press = now;
		}

		if (m_left_key_pressed && (now - last_left_key_press) >= delay) {
			last_left_key_press = now;
		}

		if (m_right_key_pressed && (now - last_right_key_press) >= delay) {
			last_right_key_press = now;
		}
	}

	void control_interface::set_open(bool value) {
		m_open = value;
	}

	void control_interface::set_option_selected(int option) {
		if (option < 0) {
			return;
		}

		if (option >= global::submenu_handler->m_current_submenu->m_options.size()) {
			return;
		}
		m_selected_option = option;
	}

	void control_interface::set_max_visible_options(int value) {
		m_max_visible_options = value;
	}

	void control_interface::set_all_options(int value) {
		m_all_options = value;
	}

	int control_interface::get_selected_option() {
		return m_selected_option;
	}

	int control_interface::get_all_options() {
		return m_all_options;
	}

	int control_interface::get_max_visible_options() {
		return m_max_visible_options;
	}

	void control_interface::reset_input() {
		m_open_key_pressed = false;
		m_back_key_pressed = false;
		m_enter_key_pressed = false;
		m_up_key_pressed = false;
		m_down_key_pressed = false;
		m_left_key_pressed = false;
		m_right_key_pressed = false;
	}
}