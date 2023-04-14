#pragma once
#include "..//submenu.hpp"
#include "option.hpp"
#include <functional>

namespace based::gui
{
	class menu : public option {
	public:
		explicit menu(const char* name);

		template<typename T>
		menu add_submenu() {
			m_submenu = T::get();
			return *this;
		}

		menu add_rage_on_click(std::function<void()> function);
		menu add_regular_on_click(std::function<void()> function);
		menu add_regular_on_hover(std::function<void()> function);
		menu add_rage_on_hover(std::function<void()> function);
		menu add_tooltip(const char* tooltip);

		void render(const int index);
	private:

		bool m_active_tooltip{ false };
		bool m_active_rage_hover{ false };
		bool m_active_rage_on_click{ false };
		bool m_active_regular_on_click{ false };
		bool m_active_regular_hover{ false };

		submenu* m_submenu = nullptr;

		std::function<void()> m_rage_click{ []() {} };
		std::function<void()> m_regular_click{ []() {} };
		std::function<void()> m_regular_on_hover{ []() {} };
		std::function<void()> m_rage_on_hover{ []() {} };
	};
}