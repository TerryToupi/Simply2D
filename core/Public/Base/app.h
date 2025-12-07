#pragma once

#include "assets.h"

#include "Rendering/renderer.h"
#include "Audio/audio.h"
#include "Scene/scene.h"

#include "Types/Arrays.h"
#include "Types/SmartPointers.h"
#include "Types/String.h"

#include "Input/KeyboardInput.h"
#include "Input/MouseInput.h"

namespace Simply2D
{
	struct ApplicationSpecifications
	{
		TString name = "Simply2D";
		TString version = "1.0";
		TString identifier = "com.terrytoupi.simply2D-empty";
		RendererSpecifications renderer;
		AssetDatabaseSpecifications assets;
		AudioSystemSpecifications audio;
	};

	class Application
	{
	public:
		static void Create(const ApplicationSpecifications& specs = ApplicationSpecifications());
		static void Destroy();

		void Run();
		void Stop();

		static inline Application* GetInstance() { return s_pInstance; }
		static inline Renderer* GetRenderer() { return s_pRenderer; }
		static inline AssetDatabase* GetAssetDatabase() { return s_pAssetDatabase; }
		static inline AudioSystem* GetAudioSystem() { return s_pAudioSystem; }

		template<std::derived_from<Scene> TScene>
		void pushScene(const std::string& scene)
		{
			m_scenes.push_back(MakeRef<TScene>(scene));
		}

		template<typename TScene>
		requires(std::is_base_of_v<Scene, TScene>)
		void activateScene()
		{
			for (unsigned i = 0; i < m_scenes.size(); ++i)
			{
				if (typeid(m_scenes[i]) == typeid(TScene))
				{
					m_activeScene = i;
					return;
				}
			}
		}

		static bool IsPressed(Keyboard code);

		Application() = default;
		~Application() = default;
		Application(const Application& app) = delete;
		Application& operator=(const Application&) = delete;

	private:
		bool m_running = false;

		ApplicationSpecifications	m_specifications;

		TVector<Ref<Scene>>			m_scenes;
		uint32_t					m_activeScene = 0;

		static inline Application*		s_pInstance = nullptr;
		static inline Renderer*			s_pRenderer = nullptr;
		static inline AssetDatabase*	s_pAssetDatabase = nullptr;
		static inline AudioSystem*		s_pAudioSystem = nullptr;

		static inline const bool*	s_pKeyState = nullptr;
		static inline int			s_Keylength = 0;
	};
}