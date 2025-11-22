#include "Base/app.h"
#include "Memory/memory.h"
#include "Memory/memoryAllocator.h"

#include "Source/Base/assetDataBase.h"
#include "Source/Base/gameTime.h"
#include "Source/Base/threadPool.h"
#include "Source/Rendering/renderingBackend.h"
#include "Source/Animation/animatorManager.h"
#include "Source/Systems/colisionChecker.h"

#include <SDL3/SDL.h>
#include <SDL3_Mixer/SDL_mixer.h>
#include <SDL3_ttf/SDL_ttf.h>

namespace Simply2D
{
	void Application::Create(const ApplicationSpecifications& specs)
	{
		assert(s_pInstance == nullptr);
		s_pInstance = New<Application>();

		s_pInstance->m_specifications = specs;

		if (!SDL_SetAppMetadata(s_pInstance->m_specifications.name.c_str(),
			s_pInstance->m_specifications.version.c_str(),
			s_pInstance->m_specifications.identifier.c_str()))
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

		ThreadPool::Initialize();
		s_pInstance->m_pRenderer = Renderer::Create(specs.renderer);
		s_pInstance->m_pAssetDatabase = AssetDatabase::Create(specs.assets);
		Allocator::GetInstance().SetRegionsCapacity(specs.memory);

		s_pInstance->m_running = true;
	}

	void Application::Run()
	{
		double oldTime = 0;
		double currTime = 0;
		float timeStep = 0;
		uint32_t frameActiveScene = 0;

#if not defined(NDEBUG)
		float statsTimer = 5.0f;
		float statsCounter = 5.0f;
#endif // !NDEBUG


		// setup timer
		currTime = Clock::getTime();
		timeStep = std::min(0.1f, std::max(0.0001f, (float)(currTime - oldTime)));
		oldTime = currTime;

		RendererImpl* renderer = static_cast<RendererImpl*>(m_pRenderer);
		while (m_running)
		{
			currTime = Clock::getTime();
			timeStep = std::min(0.1f, std::max(0.0001f, (float)(currTime - oldTime)));
			oldTime = currTime;

			// cache the currently active scene
			frameActiveScene = m_activeScene;

			// polling events
			{
				SDL_Event event;
				while (SDL_PollEvent(&event))
				{
					if (event.type == SDL_EVENT_QUIT)
						Stop();
				}
			}

			// on begin frame
			{
				// begin for the active scene
				m_scenes.at(frameActiveScene)->begin(timeStep);
			}

			// updating layers
			{
				// updating the Animations
				AnimatorManager::GetInstance().Progress(currTime);

				// check for colisions
				ColisionChecker::check(m_scenes.at(frameActiveScene)->GetSpriteRegister());

				// update the scene
				m_scenes.at(frameActiveScene)->update(timeStep);
			}

			// rendering
			renderer->beginFrame();
			{
				// rendering the scenes first
				m_scenes.at(frameActiveScene)->render();
			}
			renderer->endFrame();

			// on enf frame
			{
				m_scenes.at(frameActiveScene)->end(timeStep);

				Allocator::ResetFrameRegion();
			}

#if not defined(NDEBUG)
			if (statsCounter >= statsTimer)
			{
				statsCounter = 0.0f;
				MM::CLIReportStatistics();
			}
			else
				statsCounter += timeStep;
#endif // !NDEBUG
		}
	}

	void Application::Stop()
	{
		m_running = false;
	}

	void Application::Destroy()
	{
		// shutdown scenes
		s_pInstance->m_scenes.clear();

		// shutdown Renderer
		RendererImpl* rimpl = static_cast<RendererImpl*>(s_pInstance->m_pRenderer);
		Delete<RendererImpl>(rimpl);

		// shutdown AssetDatabase
		AssetDatabaseImpl* aimpl = static_cast<AssetDatabaseImpl*>(s_pInstance->m_pAssetDatabase);
		Delete<AssetDatabaseImpl>(aimpl);

		// shutdown SDL
		TTF_Quit();
		MIX_Quit();
		SDL_Quit();

		// shutdown Threads
		ThreadPool::Shutdown();

		// Delete the application
		Delete<Application>(s_pInstance);
		s_pInstance = nullptr;
	}
}
