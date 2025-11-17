#pragma once

#include <future>
#include <memory>
#include <functional>
#include <type_traits>

namespace Simply2D 
{
	namespace ThreadPool
	{
		using Job = std::function<void()>;

		// Initialization of the threadPool
		void Initialize();

		// Shutdown of the threadPool
		void Shutdown();
		
		// Execute job without a promise
		void Execute(const Job& job);

		// is the queue busy
		bool Busy();

		// Wait if the queue is busy to finish execution
		void Wait();

		// executing a task async and getting a result
		template<class F, class... Args>
		auto Async(F&& f, Args&&... args)
			-> std::future<typename std::invoke_result_t<F, Args...>>
		{
			using return_type = std::invoke_result_t<F, Args...>;

			auto job = std::make_shared<std::packaged_task<return_type()>>(
				[f = std::forward<F>(f), ...args = std::forward<Args>(args)]() mutable
				{
					return std::invoke(f, args...);
				}
			);

			std::future<return_type> res = job->get_future();
			Execute([job]()
				{
					(*job)();
				});
			return res;
		}
	}
}