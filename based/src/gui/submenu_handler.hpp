#pragma once
#include "gui.hpp"
#include "submenu.hpp"

namespace based::gui
{
	class submenu_handler
	{
	public:
		void load();
		void update();

		void add_submenu(submenu* submenu);

		void set_submenu(submenu* submenu);
		void set_submenu_previous();
		void cleanup();

		submenu* m_current_submenu{ nullptr };
	private:
		submenu* m_main_submenu{ nullptr };
		submenu* m_next_submenu{ nullptr };
		submenu* m_previous_submenu{ nullptr };

		std::vector<submenu*> m_submenus = {};
	};

	
}