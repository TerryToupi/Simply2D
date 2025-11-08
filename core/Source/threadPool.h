#pragma once

#include <functional>

namespace Simply2D 
{
	namespace ThreadPool
	{
		using Job = std::function<void()>;

		void	Initialize();
		void	Shutdown();
		void	Execute(const Job& job);
		bool	Busy();
		void	Wait();
	}
}