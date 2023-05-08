#include "button.hpp"
#include "..//settings.hpp"
#include "..//..//global.hpp"
#include "..//..//hooking/d3d/renderer.hpp"
#include "../../util/functions.hpp"
#include "../../util/fiberpool/fiber_pool.hpp"

namespace based::gui
{
	button::button(const char* name)
		: option(name)
	{}

	button button::add_rage_on_click(std::function<void()> function) {
		m_rage_click = function;
		m_active_rage_on_click = true;
		return *this;
	}

	button button::add_rage_on_hover(std::function<void()> function) {
		m_rage_click = function;
		m_active_rage_hover = true;
		return *this;
	}

	button button::add_regular_on_click(std::function<void()> function) {
		m_regular_click = function;
		m_active_regular_on_click = true;
		return *this;
	}

	button button::add_regular_on_hover(std::function<void()> function) {
		m_regular_on_hover = function;
		m_active_regular_hover = true;
		return *this;
	}

	button button::add_tooltip(const char* tooltip) {
		m_active_tooltip = true;
		return *this;
	}

	void button::render(const int index) {
		const auto is_selected = index == global::control->get_selected_option() ? true : false;
		
		if (global::control->is_option_pressed(index)) {
			if (m_active_regular_on_click) {
				m_regular_click();
			}
			if (m_active_rage_on_click) {
				g_fiber_pool->queue_job([&] { m_rage_click(); });
			}
		}

		if (global::control->is_option_hovered(index)) {
			if (m_active_regular_hover) {
				m_regular_on_hover();
			}
			if (m_active_rage_hover) {
				g_fiber_pool->queue_job([&] { m_rage_on_hover(); });
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
		
		draw_base += settings::option::rect_size.y;
	}
}