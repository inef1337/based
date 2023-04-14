#pragma once
#include "..//gui/gui.hpp"
#include "..//gui/submenu.hpp"

namespace based::gui
{
	class main_menu : public submenu
	{
	public:
		static main_menu* get();

		void load() override;

		main_menu()
			: submenu()
		{}
	};
}