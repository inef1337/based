#pragma once

#include "../../dependencies/imgui/imgui.h"
#include "types/option.hpp"
#include <stack>
#include <vector>
#include <memory>

namespace based::gui
{
	class submenu
	{
	public:
		const char* m_title{};

		void set_title(const char* title);

		submenu* get_submenu_parent();

		virtual void load();

		submenu* m_submenu_parent{ nullptr };

		std::vector<std::shared_ptr<option>> m_options{};
		std::stack<const char*> m_submenu_name{};

		template<typename T>
		std::shared_ptr<T> push(T&& option);

		template<typename T>
		void set_parent();

		void update_menu();
	};

	template<typename T>
	inline std::shared_ptr<T> submenu::push(T&& option) {
		std::shared_ptr<T> ptr = std::make_shared<T>(std::forward<T>(option));
		m_options.push_back(ptr);
		return ptr;
	}

	template<typename T>
	inline void submenu::set_parent() {
		m_submenu_parent = T::get();
		m_submenu_name = {};

		m_submenu_name.push(m_title);

		submenu* submenu = m_submenu_parent;
		while (submenu)
		{
			m_submenu_name.push(submenu->m_title);
			submenu = submenu->get_submenu_parent();
		}
	}
}