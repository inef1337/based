#include "player_menu.hpp"
#include "..//..//global.hpp"
#include "../../gui/types/menu.hpp"
#include "../../gui/types/toggle.hpp"
#include "../../gui/types/button.hpp"
#include "../main_menu.hpp"
#include "animation/animation_menu.hpp"
#include "../../rage/natives.hpp"
#include "..//..//util/functions.hpp"
#include <chrono>
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
		push(button("spawn ped")
			.add_rage_on_click([] {
				const auto player = PLAYER::PLAYER_PED_ID();
				const auto ped_hash = util::gta::joaat("player_zero");
				const auto coords = ENTITY::GET_ENTITY_COORDS(player, true);
				const auto ped = util::gta::create_ped(ped_hash, coords);
			})
		);
		push(button("spawn vehicle")
			.add_rage_on_click([] {
				const auto player = PLAYER::PLAYER_PED_ID();
				const auto veh_hash = util::gta::joaat("adder");
				const auto coords = ENTITY::GET_ENTITY_COORDS(player, true);
				const auto heading = ENTITY::GET_ENTITY_HEADING(player);
				const auto veh = util::gta::create_vehicle(veh_hash, coords, heading);
			})
		);
		push(button("spawn object")
			.add_rage_on_click([] {
				const auto player = PLAYER::PLAYER_PED_ID();
				const auto obj_hash = util::gta::joaat("lts_prop_lts_ramp_01");
				const auto coords = ENTITY::GET_ENTITY_COORDS(player, true);
				const auto obj = util::gta::create_object(obj_hash, coords);
			})
		);
	}
}

/*

*/