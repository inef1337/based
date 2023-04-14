#include "submenu.hpp"
#include "types/button.hpp"
#include "gui.hpp"
#include "control_interface.hpp"
#include "..//global.hpp"
#include "../rage/natives.hpp"

namespace based::gui
{
    void submenu::load()
    {
    }

    void submenu::set_title(const char* title) {
        if (title && strlen(title) > 0) {
            this->m_title = title;
        }
    }

    submenu* submenu::get_submenu_parent() {
        return m_submenu_parent;
    }

    void submenu::update_menu() {
        ImGui::SetNextWindowPos(window_position);
        ImGui::SetNextWindowSize(ImVec2(1920.f, 1080.f));
        ImGui::Begin("main", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoBackground);
        draw_base = 100.f;

        global::control->get_input();
        global::control->handle_input();
        global::control->set_all_options(this->m_options.size());
        
        if (global::control->is_open()) {

            header();
            submenubar(m_title);
            if (m_options.size() > 0) {
                const auto all_options = global::control->get_all_options();
                const auto max_visible_options = global::control->get_max_visible_options();
                const auto selected_option = global::control->get_selected_option();
                const auto is_within_scroll_range = all_options > max_visible_options && selected_option >= max_visible_options;

                auto start_point = 0;
                auto end_point = all_options > max_visible_options ? max_visible_options : all_options;

                if (is_within_scroll_range) {
                    start_point = selected_option - max_visible_options + 1;
                    end_point = selected_option + 1;
                }

                for (auto i = start_point, j = 0; i < end_point; ++i, ++j) {
                    m_options.at(i)->render(i);
                }
            }
            footer();
        }
        ImGui::End();
    }
}