#pragma once
#include "..//..//..//gui/submenu.hpp"

namespace based::gui
{
	class animation_menu : public submenu
	{
	public:
		static animation_menu* get();

		void load() override;

		animation_menu()
			: submenu()
		{}
	};
}