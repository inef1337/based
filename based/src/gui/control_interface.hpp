#pragma once
#include <cstdint>

namespace based::gui
{
	class control_interface
	{
	public:
		control_interface() noexcept = default;
		~control_interface() noexcept = default;

		bool is_open_key_pressed() const;
		bool is_back_key_pressed() const;
		bool is_enter_key_pressed() const;
		bool is_up_key_pressed() const;
		bool is_down_key_pressed() const;
		bool is_left_key_pressed() const;
		bool is_right_key_pressed() const;
		bool is_option_selected(int index);

		bool is_open();
		bool is_option_pressed(const int index);
		bool is_option_hovered(const int index);

		void get_input();
		void handle_input();
		void set_open(bool value);
		void set_option_selected(int option);
		void set_max_visible_options(int value);
		void set_all_options(int value);

		int get_selected_option();
		int get_all_options();
		int get_max_visible_options();

	private:
		void reset_input();

		bool is_key_pressed(std::uint16_t key);

		bool m_open{ true };
		bool m_open_key_pressed{ false };
		bool m_back_key_pressed{ false };
		bool m_enter_key_pressed{ false };
		bool m_up_key_pressed{ false };
		bool m_down_key_pressed{ false };
		bool m_left_key_pressed{ false };
		bool m_right_key_pressed{ false };

		int m_selected_option{ 0 };
		int m_max_visible_options{ 8 };
		int m_all_options{};
	};
}