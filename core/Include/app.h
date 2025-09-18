#pragma once

#include <string>
#include <window.h>

namespace core
{
	struct ApplicationSpecifications
	{
		std::string name;
		WindowSpecifications window;
	};

	class Application
	{
	public:
		Application(const ApplicationSpecifications& specs = ApplicationSpecifications());
		~Application();

		void run();

	private:
		ApplicationSpecifications m_specifications;
		std::unique_ptr<Window> m_window;
	};
}