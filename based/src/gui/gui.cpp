#include "gui.hpp"
#include "..//hooking/d3d/renderer.hpp"
#include "settings.hpp"
#include "../util/functions.hpp"
#include <stack>
#include <vector>
#include <memory>



namespace based::gui
{
    void header()
    {
        global::render->rect(
            pos_x,
            draw_base,
            settings::header::rect_size.x,
            settings::header::rect_size.y,
            settings::header::rect_color
        );

        const auto pos = center_text(base_name, pos_x, draw_base, settings::header::rect_size.x, settings::header::rect_size.y, 
            settings::header::text_size, util::alignment::CENTER);

        global::render->text(
            base_name.c_str(),
            pos.x,
            pos.y,
            settings::header::text_color,
            settings::header::font
        );

        draw_base += settings::header::rect_size.y;
    }

    void submenubar(std::string text)
    {
        global::render->rect(
            pos_x,
            draw_base,
            settings::submenubar::rect_size.x,
            settings::submenubar::rect_size.y,
            settings::submenubar::rect_color
        );

        auto description_pos = center_text(base_name, pos_x, draw_base, settings::submenubar::rect_size.x, settings::submenubar::rect_size.y,
            settings::submenubar::text_size, util::alignment::LEFT);

        description_pos.x += settings::submenubar::description_padding;

        util::mutate_case(text, util::cases::UPPER);

        global::render->text(
            text.c_str(),
            description_pos.x,
            description_pos.y,
            settings::submenubar::text_color,
            settings::submenubar::font
        );

        const auto selected_option = global::control->get_selected_option();
        const auto all_options = global::control->get_all_options();
        const auto option_count = std::to_string(selected_option == all_options ? selected_option : selected_option + 1) + "/" + std::to_string(all_options);

        auto option_count_pos = center_text(option_count, pos_x, draw_base, settings::submenubar::rect_size.x, settings::submenubar::rect_size.y,
            settings::submenubar::text_size, util::alignment::RIGHT);

        option_count_pos.x -= settings::submenubar::option_count_padding;

        global::render->text(
            option_count.c_str(),
            option_count_pos.x,
            option_count_pos.y,
            settings::submenubar::text_color,
            settings::submenubar::font
        );

        draw_base += settings::submenubar::rect_size.y;
    }

    void footer()
    {
        global::render->rect(
            pos_x,
            draw_base,
            settings::footer::rect_size.x,
            settings::footer::rect_size.y,
            settings::footer::rect_color
        );

        auto pos = center_text(base_name, pos_x, draw_base, settings::footer::rect_size.x, settings::footer::rect_size.y,
            settings::footer::arrow_size.x, util::alignment::CENTER);
       
        pos.x += settings::footer::padding;

        global::render->texture(
           settings::footer::image,
           pos.x,
           pos.y,
           settings::footer::arrow_size.x,
           settings::footer::arrow_size.y,
           settings::footer::arrow_color
        );

        draw_base += settings::footer::rect_size.y;
    }
}

