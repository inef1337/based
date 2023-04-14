#pragma once
#include "option.hpp"
#include <functional>

namespace based::gui
{
	class toggle : public option {
	public:
		explicit toggle(const char* name);

		toggle add_rage_on_click(std::function<void()> function);
		toggle add_regular_on_click(std::function<void()> function);
		toggle add_regular_on_hover(std::function<void()> function);
		toggle add_rage_on_hover(std::function<void()> function);
		toggle add_tooltip(const char* tooltip);
		toggle add_toggle(bool& toggle);


		void render(const int index);
	private:
		bool* m_toggle = nullptr;

		bool m_active_tooltip{ false };
		bool m_active_rage_hover{ false };
		bool m_active_rage_on_click{ false };
		bool m_active_regular_on_click{ false };
		bool m_active_regular_hover{ false };

		std::function<void()> m_rage_click{ []() {} };
		std::function<void()> m_regular_click{ []() {} };
		std::function<void()> m_regular_on_hover{ []() {} };
		std::function<void()> m_rage_on_hover{ []() {} };
	};
}