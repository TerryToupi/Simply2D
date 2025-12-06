#include "Base/app.h"
#include "Memory/memory.h"

#include "Source/Base/assetDataBase.h"
#include "Source/Base/gameTime.h"
#include "Source/Rendering/renderingBackend.h"
#include "Source/Animation/animatorManager.h"
#include "Source/Systems/colisionChecker.h"

#include "Events/event.h"
#include "Events/inputEvents.h"
#include "Events/windowEvent.h"

#include <SDL3/SDL.h>
#include <SDL3_Mixer/SDL_mixer.h>
#include <SDL3_ttf/SDL_ttf.h>

namespace Simply2D
{
	void Application::Create(const ApplicationSpecifications& specs)
	{
		assert(s_pInstance == nullptr);
		s_pInstance = MM::New<Application>();

		s_pInstance->m_specifications = specs;

		if (!SDL_SetAppMetadata(s_pInstance->m_specifications.name.c_str(),
			s_pInstance->m_specifications.version.c_str(),
			s_pInstance->m_specifications.identifier.c_str()))
			SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "[SDL] failed to initialize %s", SDL_GetError());

		if (!SDL_Init(SDL_INIT_VIDEO |
			SDL_INIT_EVENTS |
			SDL_INIT_AUDIO |
			SDL_INIT_GAMEPAD))
			SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "[SDL] failed to initialize %s", SDL_GetError());

		if (!MIX_Init())
			SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "[SDL] failed to initialize %s", SDL_GetError());

		if (!TTF_Init())
			SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "[SDL] failed to initialize %s", SDL_GetError());

		AnimatorManager::Create(); 
		s_pInstance->s_pRenderer = Renderer::Create(specs.renderer);
		s_pInstance->s_pAssetDatabase = AssetDatabase::Create(specs.assets);
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

		// Start
		{
			for (const auto& scene : m_scenes)
			{
				scene->start();
			}
		}

		RendererImpl* renderer = static_cast<RendererImpl*>(s_pRenderer);
		while (m_running)
		{
			currTime = Clock::getTime();
			timeStep = std::min(0.1f, std::max(0.0001f, (float)(currTime - oldTime)));
			oldTime = currTime;

			// cache the currently active scene
			frameActiveScene = m_activeScene;

			// Init keyboard scan codes;
			{
				s_pKeyState = SDL_GetKeyboardState(&s_Keylength);
			}

			// polling events
			{
				SDL_Event event;
				while (SDL_PollEvent(&event))
				{
					if (event.type == SDL_EVENT_QUIT)
					{
						WindowClosedEvent e{};
						m_scenes.at(frameActiveScene)->event(e);
						Stop();
					}
					if (event.type == SDL_EVENT_KEY_DOWN)
					{
						KeyPressedEvent e{ (int)event.key.key, false};
						m_scenes.at(frameActiveScene)->event(e);
					}
					if (event.type == SDL_EVENT_KEY_UP)
					{
						KeyReleasedEvent e{ (int)event.key.key};
						m_scenes.at(frameActiveScene)->event(e);
					}
					if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN)
					{
						MouseButtonPressedEvent e{ (int)event.button.button };
						m_scenes.at(frameActiveScene)->event(e);
					}
					if (event.type == SDL_EVENT_MOUSE_BUTTON_UP)
					{
						MouseButtonReleasedEvent e{ (int)event.button.button };
						m_scenes.at(frameActiveScene)->event(e);
					}
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
				AnimatorManager::GetInstance()->Progress(currTime);

				// collision checking
				//ColisionChecker::check(m_scenes.at(frameActiveScene));

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

			// on end frame
			{
				m_scenes.at(frameActiveScene)->end(timeStep);
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

	bool Application::IsPressed(Keyboard code)
	{
		auto kcode = static_cast<SDL_Scancode>(code);
		assert(kcode < s_Keylength);
		return s_pKeyState[kcode];
	}

	void Application::Destroy()
	{
		// shutdown scenes
		s_pInstance->m_scenes.clear();

		// shutdown Renderer
		auto rimpl = static_cast<RendererImpl*>(s_pInstance->s_pRenderer);
		MM::Delete<RendererImpl>(rimpl);

		// shutdown AssetDatabase
		auto aimpl = static_cast<AssetDatabaseImpl*>(s_pInstance->s_pAssetDatabase);
		MM::Delete<AssetDatabaseImpl>(aimpl);

		// delete animator manager
		AnimatorManager::Destroy();

		// shutdown SDL
		TTF_Quit();
		MIX_Quit();
		SDL_Quit();

		// Delete the application
		MM::Delete<Application>(s_pInstance);
		s_pInstance = nullptr;
	}
}
