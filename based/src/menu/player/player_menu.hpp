#pragma once
#include "..//..//gui/submenu.hpp"

namespace based::gui
{
	class player_menu : public submenu
	{
	public:
		static player_menu* get();

		void load() override;

		player_menu()
			: submenu()
		{}
	};
}