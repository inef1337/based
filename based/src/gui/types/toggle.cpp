#include "toggle.hpp"
#include "..//settings.hpp"
#include "..//..//global.hpp"
#include "..//..//hooking/d3d/renderer.hpp"
#include "../../util/functions.hpp"

namespace based::gui
{
	toggle::toggle(const char* name)
		: option(name)
	{

	}

	toggle toggle::add_rage_on_click(std::function<void()> function) {
		m_rage_click = function;
		m_active_rage_on_click = true;
		return *this;
	}

	toggle toggle::add_rage_on_hover(std::function<void()> function) {
		m_rage_click = function;
		m_active_rage_hover = true;
		return *this;
	}

	toggle toggle::add_regular_on_click(std::function<void()> function) {
		m_regular_click = function;
		m_active_regular_on_click = true;
		return *this;
	}

	toggle toggle::add_regular_on_hover(std::function<void()> function) {
		m_regular_on_hover = function;
		m_active_regular_hover = true;
		return *this;
	}

	toggle toggle::add_tooltip(const char* tooltip) {
		m_active_tooltip = true;
		return *this;
	}

	toggle toggle::add_toggle(bool& toggle) {
		m_toggle = &toggle;
		return *this;
	}
	
	void toggle::render(const int index) {
		const auto is_selected = index == global::control->get_selected_option() ? true : false;

		if (global::control->is_option_pressed(index)) {
			if (m_toggle) {
				*m_toggle = !*m_toggle;
			}
			if (m_active_regular_on_click) {
				m_regular_click();
			}
			if (m_active_rage_on_click) {
				global::queue->job([&] { m_rage_click(); });
			}
		}

		if (global::control->is_option_hovered(index)) {
			if (m_active_regular_hover) {
				m_regular_on_hover();
			}
			if (m_active_rage_hover) {
				global::queue->job([&] { m_rage_on_hover(); });
			}
		}

		global::render->rect(
			pos_x,
			draw_base,
			settings::option::rect_size.x,
			settings::option::rect_size.y,
			is_selected ? settings::option::rect_color_selected : settings::option::rect_color_unselected
		);

		auto pos = center_text(m_name, pos_x, draw_base, settings::option::rect_size.x, settings::option::rect_size.y,
			settings::option::text_size, util::alignment::LEFT);

		pos.x += settings::option::padding;

		global::render->text(
			m_name,
			pos.x,
			pos.y,
			is_selected ? settings::option::text_color_selected : settings::option::text_color_unselected,
			settings::option::font
		);

		auto toggle_pos = center_text(base_name, pos_x, draw_base, settings::option::rect_size.x, settings::option::rect_size.y,
			settings::option::toggle_size.x, util::alignment::RIGHT);

		toggle_pos.x += settings::option::toggle_padding;

		if (m_toggle) {
			global::render->texture(
				*m_toggle ? settings::option::ticked_toggle_image : settings::option::toggle_image,
				toggle_pos.x,
				toggle_pos.y,
				settings::option::toggle_size.x,
				settings::option::toggle_size.y,
				*m_toggle ? is_selected ? settings::option::toggle_color : settings::option::toggle_color : is_selected ? settings::option::ticked_toggle_color : settings::option::toggle_color
			);
			
			if (is_selected && *m_toggle) { 
				global::render->texture(
					settings::option::toggle_image,
					toggle_pos.x - 1.f,
					toggle_pos.y - 1.f,
					settings::option::toggle_size.x + 2.f,
					settings::option::toggle_size.y + 2.f,
					settings::option::ticked_toggle_color
				);
			}
		}

		draw_base += settings::option::rect_size.y;
	}
}