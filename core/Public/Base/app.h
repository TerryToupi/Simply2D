#pragma once

#include "assets.h"

#include "Rendering/renderer.h"
#include "Scene/scene.h"

#include "Types/Arrays.h"
#include "Types/SmartPointers.h"
#include "Types/String.h"

namespace Simply2D
{
	struct ApplicationSpecifications
	{
		TString name = "Simply2D";
		TString version = "1.0";
		TString identifier = "com.terrytoupi.simply2D-empty";
		RendererSpecifications renderer;
		AssetDatabaseSpecifications assets;
	};

	class Application
	{
	public:
		static void Create(const ApplicationSpecifications& specs = ApplicationSpecifications());
		static void Destroy();

		void Run();
		void Stop();

		static Application* GetInstance()
		{
			return s_pInstance;
		}

		Renderer* GetRenderer()
		{
			return m_pRenderer;
		}
		AssetDatabase* GetAssetDatabase()
		{
			return m_pAssetDatabase;
		}

		template<std::derived_from<Scene> TScene>
		void pushScene(Asset asset)
		{
			m_scenes.push_back(MakeRef<TScene>(asset));
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

		Application() = default;
		~Application() = default;
		Application(const Application& app) = delete;
		Application& operator=(const Application&) = delete;

	private:
		bool m_running = false;

		ApplicationSpecifications	m_specifications;
		Renderer*					m_pRenderer;
		AssetDatabase*				m_pAssetDatabase;

		TVector<Ref<Scene>>			m_scenes;
		uint32_t					m_activeScene = 0;

		static inline Application* s_pInstance = nullptr;
	};
}