#pragma once

#include "..//global.hpp"

#include <iostream>
#include <Windows.h>
#include <cstdint>
#include <vector>
#include <sstream>
#include <fstream>
#include <string>
#include <algorithm>
#include <filesystem>
#include <memory>
#include "../../dependencies/imgui/imgui.h"

namespace based::util
{
    inline bool is_key_pressed(int key_code)
    {
        return GetAsyncKeyState(key_code) & 0x8000;
    }

    inline bool is_valid_ptr(void* ptr) {
        return ptr != nullptr;
    }

    inline LPVOID check_ptr(const char* label, LPVOID sig) {
        if (!is_valid_ptr(sig)) {
            char buffer[256];
            sprintf_s(buffer, "failed to find %s", label);
            throw std::runtime_error(buffer);
        }
        global::console->log("found {} at {}", label, sig);
        return sig;
    }

    enum class alignment { LEFT, CENTER, RIGHT };
    inline ImVec2 center_text(
        const std::string& text,
        const float rect_x,
        const float rect_y,
        const float rect_width,
        const float rect_height,
        const float font_size,
        const alignment align
    )
    {
        const ImFont* font = ImGui::GetFont();
        const ImVec2 text_size = font->CalcTextSizeA(font_size, FLT_MAX, 0, text.c_str());
        int x{};
        int y{};

        switch (align)
        {
        case alignment::LEFT:
            x = rect_x;
            break;
        case alignment::CENTER:
            x = rect_x + (rect_width - text_size.x) / 2;
            break;
        case alignment::RIGHT:
            x = rect_x + rect_width - text_size.x;
            break;
        }

        y = rect_y + (rect_height - text_size.y) / 2;

        return ImVec2(x, y);
    }


    enum class cases { UPPER, LOWER };
    inline void mutate_case(std::string& str, const cases type) {
        if (type == cases::UPPER) {
            std::transform(str.begin(), str.end(), str.begin(), ::toupper);
        }
        else {
            std::transform(str.begin(), str.end(), str.begin(), ::tolower);
        }
    }

}