#include <cstdint>
#include "..//..//global.hpp"
#include "..//hooking.hpp"
#include "../../rage/natives.hpp"
#include "..//..//util/script_mgr.hpp"
#include "..//..//util/functions.hpp"
#include "../pointers.hpp"

namespace based::hooks
{
	int m_frame_count{0};

	bool run_script_threads(std::uint32_t ops_to_execute) {
		
		g_native_invoker.cache_handlers();

		if (global::alive) {
			execute_script(util::gta::joaat("main_persistent"), [] {	
				g_script_mgr.tick();
			});
		}

		return static_cast<decltype(&run_script_threads)>(global::hooking_mgr->m_run_script_threads->m_original)(ops_to_execute);
	}
}