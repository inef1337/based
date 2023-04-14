#include "player_menu.hpp"
#include "..//..//global.hpp"
#include "../../gui/types/menu.hpp"
#include "../../gui/types/toggle.hpp"
#include "../../gui/types/button.hpp"
#include "../main_menu.hpp"
#include "animation/animation_menu.hpp"
#include "../../rage/natives.hpp"

namespace based::gui
{
	player_menu* player_menu::get() {
		static player_menu* instance{};
		if (instance == nullptr) {
			instance = new player_menu();
			instance->load();
			instance->set_title("Player");
			instance->set_parent<main_menu>();
			global::submenu_handler->add_submenu(instance);
		}
		return instance;
	}

	bool a{};

	void player_menu::load()
	{
		push(menu("Animations")
			.add_submenu<animation_menu>()
		);
		push(toggle("toggle option")
			.add_toggle(a)
			.add_regular_on_click([] {
				global::console->log("you pressed the toggle option, the value of the boolean is now {}", a);
			})
		);
		push(button("set ped invincible")
			.add_rage_on_click([] {
				ENTITY::SET_ENTITY_INVINCIBLE(PLAYER::PLAYER_PED_ID(), true);
			})
		);
		push(button("disable ped invincible")
			.add_rage_on_click([] {
				ENTITY::SET_ENTITY_INVINCIBLE(PLAYER::PLAYER_PED_ID(), false);
			})
		);
		push(button("Option 4"));
	}
}