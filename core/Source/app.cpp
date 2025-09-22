#include <pch.h>
#include <Include/app.h>

#include <Source/renderingBackend.h>
#include <Source/assetDataBase.h>
#include <Source/mtJobSystem.h>

#include <SDL3/SDL.h>
#include <SDL3_Mixer/SDL_mixer.h>
#include <SDL3_ttf/SDL_ttf.h>

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

		if (!MIX_Init())
			SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "[SDL] failed to inialize %s", SDL_GetError());

		if (!TTF_Init()) 
			SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "[SDL] failed to inialize %s", SDL_GetError());

		JobSystem::Initialize();
		m_renderer = Renderer::Create(specs.renderer);
		m_assetDatabase = AssetDatabase::Create(specs.assets);

		m_running = true;
	}

	void Application::run()
	{
		RendererImpl* renderer = static_cast<RendererImpl*>(m_renderer.get());

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
						stop(); 
				}
			}

			// updating layers
			{
				for (const auto& layer : m_layers)
					layer->update();
			}

			// rendering
			renderer->beginFrame();
			{
				for (const auto& layer : m_layers)
					layer->render();
			}
			renderer->endFrame();
		}

		// Destroying Layer memory
		{
			for (const auto& layer : m_layers)
				layer->destroy();
		}
	
		TTF_Quit();
		MIX_Quit();
		SDL_Quit();
	} 

	void Application::stop()
	{
		m_running = false;
	} 
}