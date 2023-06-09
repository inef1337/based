#include "invoker.hpp"
#include "../util/logger.hpp"
#include "..//hooking/pointers.hpp"

extern "C" void	_call_asm(void* context, void* function, void* ret);

namespace based
{
	native_call_context::native_call_context()
	{
		m_return_value = &m_return_stack[0];
		m_args = &m_arg_stack[0];
	}

	void native_invoker::cache_handlers()
	{
		if (m_handlers_cached)
			return;

		for (const rage::scrNativeMapping& mapping : g_crossmap)
		{
			rage::scrNativeHandler handler = global::ptr->m_get_native_handler(
				global::ptr->m_native_registration_table, mapping.second);
			m_handler_cache.emplace(mapping.first, handler);
		}

		m_handlers_cached = true;
	}

	void native_invoker::begin_call()
	{
		m_call_context.reset();
	}

	void native_invoker::end_call(rage::scrNativeHash hash)
	{
		if (auto it = m_handler_cache.find(hash); it != m_handler_cache.end())
		{
			rage::scrNativeHandler handler = it->second;

			[this, hash, handler]
			{
				__try
				{
					_call_asm(&m_call_context, handler, global::ptr->m_native_return);
					global::ptr->m_fix_vectors(&m_call_context);
				}
				__except (EXCEPTION_EXECUTE_HANDLER)
				{
					[hash]() { global::console->log_to_console(based::global::log_color::red, "EXCP", "Exception caught while trying to call {} native", hash); }();
				}
			}();
		}
		else
		{
			[hash]() { global::console->log_to_console(based::global::log_color::red, "EXCP", "Failed to find {}", hash); }();
		}
	}
}