#include "main_menu.hpp"
#include "../gui/types/button.hpp"
#include "../gui/types/menu.hpp"
#include "..//util/logger.hpp"
#include "../gui/types/toggle.hpp"
#include "..//global.hpp"
#include "player/player_menu.hpp"

namespace based::gui
{
	main_menu* main_menu::get() {
		static main_menu* instance{};
		if (instance == nullptr) {
			instance = new main_menu();
			instance->load();
			instance->set_title("Main");
			global::submenu_handler->add_submenu(instance);
		}

		return instance;
	}

	static bool test{};

	void main_menu::load()
	{
		push(menu("Player")
			.add_submenu<player_menu>()
		);
		push(menu("Weapon")
			.add_submenu<player_menu>()
		);
		push(menu("Vehicle")
			.add_submenu<player_menu>()
		);
		push(menu("World")
			.add_submenu<player_menu>()
		);
		push(menu("Spawner")
			.add_submenu<player_menu>()
		);
		push(menu("Teleport")
			.add_submenu<player_menu>()
		);
		push(menu("Miscellaneous")
			.add_submenu<player_menu>()
		);
		push(button("Unload")
			.add_regular_on_click([] {
				global::alive = false;
			})
		);
	}
}