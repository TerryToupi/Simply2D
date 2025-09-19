#include <pch.h>
#include <Include/app.h>
#include <Source/renderer_impl.h>
#include <SDL3/SDL.h>

namespace core
{
	void Application::create(const ApplicationSpecifications& specs)
	{
		m_specifications = specs;

		if (!SDL_SetAppMetadata(m_specifications.name.c_str(),
								m_specifications.version.c_str(),
								m_specifications.identifier.c_str()))
			SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "[SDL] failed to inialize %s", SDL_GetError());

		if (!SDL_Init(SDL_INIT_VIDEO	| 
					  SDL_INIT_EVENTS	| 
					  SDL_INIT_AUDIO	|
					  SDL_INIT_GAMEPAD))
			SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "[SDL] failed to inialize %s", SDL_GetError());

		m_renderer = Renderer::Create(specs.renderer);
		m_running = true;
	}

	void Application::run()
	{
		for (const auto& layer : m_layers)
			layer->start();

		while (m_running)
		{
			// polling events
			{
				SDL_Event event;
				while (SDL_PollEvent(&event))
				{
					if (event.type == SDL_EVENT_QUIT)
						Application::GetInstance().stop();
				}
			}

			// updating layers
			{
				for (const auto& layer : m_layers)
					layer->update();
			}

			// rendering
			{
				m_renderer->beginFrame();
				for (const auto& layer : m_layers)
					layer->render();
				m_renderer->endFrame();
			}
		}

		// Destroying Layer memory
		{
			for (const auto& layer : m_layers)
				layer->destroy();
		}
	} 

	void Application::stop()
	{
		m_running = false;
	} 

	void Application::destroy()
	{
		SDL_Quit();
	}
}