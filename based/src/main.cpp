#include "global.hpp"
#include "hooking/pointers.hpp"
#include "hooking/hooking.hpp"

#include <cstdint>
#include <ctime>
#include <iostream>
#include <Windows.h>
#include <cstdint>
#include "util/exception_handler.hpp"
#include "util/script_mgr.hpp"
#include "util/script/script.hpp"
#include "util/fiberpool/fiber_pool.hpp"

namespace based
{
    void main_thread(const HMODULE instance)
    {
        global::vectored_exception_handler = AddVectoredExceptionHandler(0, exception_handler);
        try
        {
            global::init_logger();
            global::init_pointers();
            
            //before attempting any modifications, we ensure that the game is fully loaded to prevent any errors caused by missing or incomplete data
            global::console->log("waiting for the game load...");
            while (*global::ptr->m_game_state != 0) { 
                std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            }

            global::init_submenu_handler();
            global::init_renderer(); 
            global::init_hooking();
            
            global::hooking_mgr->create_hooks();
            global::hooking_mgr->enable_hooks();

            const auto fiber_pool_instance = std::make_unique<fiber_pool>(11);

            global::init_control_interface();
            global::submenu_handler->load();

            while (global::alive) {
                global::get_input();
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
            }

            global::hooking_mgr->disable_hooks();
            global::hooking_mgr->free_hooks();
            global::submenu_handler->cleanup(); 

            //since all other instances are initialized as smart pointers and don't leave any traces in the game, they will be automatically deallocated once they are no longer referenced
        }
        catch (std::exception const& ex)
        {
            global::console->log_to_console(global::log_color::red, "exception", ex.what());
            if (global::hooking_mgr) {
                global::hooking_mgr->disable_hooks();
                global::hooking_mgr->free_hooks();
                global::submenu_handler->cleanup();
            }
            Sleep(10000);
        }

        if (global::vectored_exception_handler) {
            RemoveVectoredExceptionHandler(global::vectored_exception_handler);
        }
        
        FreeLibraryAndExitThread(instance, 0);
    }
}

int __stdcall DllMain(
    const HMODULE instance,
    const std::uintptr_t reason,
    const void* reserved
)
{
    if (reason == DLL_PROCESS_ATTACH)
    {
        DisableThreadLibraryCalls(instance);

        const auto thread = CreateThread(
            nullptr,
            0,
            reinterpret_cast<LPTHREAD_START_ROUTINE>(based::main_thread),
            instance,
            0,
            nullptr
        );

        if (thread)
            CloseHandle(thread);
    }

    return 1;
}