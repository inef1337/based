#pragma once
#include <stack>
#include <functional>
#include <mutex>

namespace based
{
	class native_fiber_queue
	{
	public:
		void job(std::function<void()> job);
		void fiber_tick();

	private:
		std::recursive_mutex m_mutex;
		std::stack<std::function<void()>> m_jobs;
	};
}