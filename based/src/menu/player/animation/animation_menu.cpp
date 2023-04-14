#include "animation_menu.hpp"
#include "..//..//..//global.hpp"
#include "../../../gui/types/button.hpp"
#include "../player_menu.hpp"

namespace based::gui
{
	animation_menu* animation_menu::get() {
		static animation_menu* instance{};
		if (instance == nullptr) {
			instance = new animation_menu();
			instance->load();
			instance->set_title("Player");
			instance->set_parent<player_menu>();
			global::submenu_handler->add_submenu(instance);
		}
		return instance;
	}

	void animation_menu::load()
	{
		push(button("Animation 1"));
		push(button("Animation 2"));
		push(button("Animation 3"));
		push(button("Animation 4"));
	}
}