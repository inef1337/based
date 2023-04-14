
#include "native_queue.hpp"
#include <mutex>
#include <thread>

namespace based
{
	void native_fiber_queue::job(std::function<void()> job) {
		if (job) {
			std::lock_guard lock(m_mutex);
			m_jobs.push(std::move(job));
		}
	}

	void native_fiber_queue::fiber_tick() {
		std::unique_lock lock(m_mutex);
		if (!m_jobs.empty()) {
			std::function<void()> job = std::move(m_jobs.top());
			m_jobs.pop();
			lock.unlock();
			std::invoke(std::move(job));
		}
	}
}