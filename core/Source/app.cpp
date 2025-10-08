#include <pch.h>

#include <app.h>
#include <memoryAllocator.h>

#include <Source/renderingBackend.h>
#include <Source/assetDataBase.h>
#include <Source/mtJobSystem.h>
#include <Source/gameTime.h>
#include <Source/animatorManager.h>

#include <SDL3/SDL.h>
#include <SDL3_Mixer/SDL_mixer.h>
#include <SDL3_ttf/SDL_ttf.h>

namespace Simply2D
{
	void Application::create(const ApplicationSpecifications& specs)
	{
		m_specifications = specs;

		if (!SDL_SetAppMetadata(m_specifications.name.c_str(),
			m_specifications.version.c_str(),
			m_specifications.identifier.c_str()))
			SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "[SDL] failed to inialize %s", SDL_GetError());

		if (!SDL_Init(SDL_INIT_VIDEO |
			SDL_INIT_EVENTS |
			SDL_INIT_AUDIO |
			SDL_INIT_GAMEPAD))
			SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "[SDL] failed to inialize %s", SDL_GetError());

		if (!MIX_Init())
			SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "[SDL] failed to inialize %s", SDL_GetError());

		if (!TTF_Init())
			SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "[SDL] failed to inialize %s", SDL_GetError());

		JobSystem::Initialize();

		m_renderer = Renderer::Create(specs.renderer);
		m_assetDatabase = AssetDatabase::Create(specs.assets);

		Allocator::GetInstance().SetRegionsCapacity(512 * 1024, 512 * 1024);

		m_running = true;
	}

	void Application::run()
	{
		double oldTime = 0;
		double currTime = 0;
		float timeStep = 0;

		RendererImpl* renderer = static_cast<RendererImpl*>(m_renderer.get());

		// setup timer
		currTime = Clock::getTime();
		timeStep = std::min(0.1f, std::max(0.0001f, (float)(currTime - oldTime)));
		oldTime = currTime;

		while (m_running)
		{
			currTime = Clock::getTime();
			timeStep = std::min(0.1f, std::max(0.0001f, (float)(currTime - oldTime)));
			oldTime = currTime;

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
				// updating the Animations
				AnimatorManager::GetInstance().Progress(currTime);
				
				// update the scripting
				for (const auto& layer : m_layers)
					layer->update(timeStep);
			}

			// rendering
			renderer->beginFrame();
			{
				for (const auto& layer : m_layers)
					layer->render();
			}
			renderer->endFrame();

			// execute main thread jobs after scripting is done
			// processing for this frame
			{
				std::function<void()> job;
				while (m_mainThreadQueue.pop_front(job))
					job();
			}
			
			Allocator::ResetFrameRegion();
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
