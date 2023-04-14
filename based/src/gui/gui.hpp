#pragma once

#include "../../dependencies/imgui/imgui.h"
#include "types/option.hpp"
#include <stack>
#include <vector>
#include <memory>

namespace based::gui
{
	inline ImVec2 window_position{ 0.f, 0.f };
	inline float draw_base{ 35.f };
	inline float width{ 375.f };
	inline float pos_x{ 70.f };

    void header();
    void submenubar(std::string text);
    void footer();



	

}