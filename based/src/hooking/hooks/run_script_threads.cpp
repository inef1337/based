#include <cstdint>
#include "..//..//global.hpp"
#include "..//hooking.hpp"
#include "../../rage/natives.hpp"

namespace based::hooks
{
	int m_frame_count{0};

	bool run_script_threads(std::uint32_t ops_to_execute) {
		
		g_native_invoker.cache_handlers();

		if (global::alive) {
			if (m_frame_count != MISC::GET_FRAME_COUNT()) {
				m_frame_count = MISC::GET_FRAME_COUNT();
				global::queue->fiber_tick();
			}

			if (global::control->is_open()) {
				PAD::DISABLE_CONTROL_ACTION(0, 27, true); //Disable phone
			}
		}

		return static_cast<decltype(&run_script_threads)>(global::hooking_mgr->m_run_script_threads->m_original)(ops_to_execute);
	}
}