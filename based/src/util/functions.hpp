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
#include "../rage/natives.hpp"
#include "script/script.hpp"
#include "..//rage/classes/classes.hpp"
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

    constexpr char to_lower(const char c) {
        return (c >= 'A' && c <= 'Z') ? c + ('a' - 'A') : c;
    }

    namespace gta {

        inline std::uint32_t joaat(const char* str)
        {
            std::uint32_t hash = 0;
            while (*str)
            {
                hash += to_lower(*str++);
                hash += (hash << 10);
                hash ^= (hash >> 6);
            }

            hash += (hash << 3);
            hash ^= (hash >> 11);
            hash += (hash << 15);

            return hash;
        }

        inline std::int32_t create_ped(const std::uint32_t hash, const Vector3 coords, const float heading = 0.f) {
           
            while (!STREAMING::HAS_MODEL_LOADED(hash)) {
                STREAMING::REQUEST_MODEL(hash);
                script::get_current()->yield();
            }

            const auto ped = PED::CREATE_PED(
                26, //mission entity
                hash, 
                coords.x, 
                coords.y, 
                coords.z, 
                heading,
                0, 
                0
            );

            script::get_current()->yield();

            STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(hash);

            return ped;
        }

        inline std::int32_t create_vehicle(const std::uint32_t hash, const Vector3 coords, const float heading = 0.f) {

            while (!STREAMING::HAS_MODEL_LOADED(hash)) {
                STREAMING::REQUEST_MODEL(hash);
                script::get_current()->yield();
            }

            const auto vehicle = VEHICLE::CREATE_VEHICLE(
                hash,
                coords.x,
                coords.y,
                coords.z,
                heading,
                0,
                0,
                0
            );

            script::get_current()->yield();

            STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(hash);

            return vehicle;
        }
        inline std::int32_t create_object(const std::uint32_t hash, const Vector3 coords, const float heading = 0.f, const bool is_dynamic = true) {

            while (!STREAMING::HAS_MODEL_LOADED(hash)) {
                STREAMING::REQUEST_MODEL(hash);
                script::get_current()->yield();
            }

            const auto object = OBJECT::CREATE_OBJECT(
                hash,
                coords.x,
                coords.y,
                coords.z,
                heading,
                0,
                is_dynamic
            );

            script::get_current()->yield();

            STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(hash);

            return object;
        }

    }
}