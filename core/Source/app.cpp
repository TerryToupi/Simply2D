#include <pch.h>
#include <Include/app.h>
#include <Source/window_impl.h>

namespace core
{
	Application::Application(const ApplicationSpecifications& specs)
		:	m_specifications(specs)
	{
		SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_EVENTS);

		m_window = Window::Create(specs.window);
	}

	Application::~Application()
	{
		SDL_Quit();
	}

	void Application::run()
	{
		// Main loop example
		bool running = true;
		while (running)
		{
			SDL_Event event;
			while (SDL_PollEvent(&event))
			{
				if (event.type == SDL_EVENT_QUIT)
					running = false;
			}
		}
	}
}