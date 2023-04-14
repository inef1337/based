#pragma once
#include "option.hpp"
#include <functional>

namespace based::gui
{
	class button : public option {
	public:
		explicit button(const char* name);

		button add_rage_on_click(std::function<void()> function);
		button add_regular_on_click(std::function<void()> function);
		button add_regular_on_hover(std::function<void()> function);
		button add_rage_on_hover(std::function<void()> function);
		button add_tooltip(const char* tooltip);

		void render(const int index);
	private:
		
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