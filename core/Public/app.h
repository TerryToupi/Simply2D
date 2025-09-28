#pragma once

#include <string>
#include <vector>
#include <layer.h>

#include <assets.h>
#include <renderer.h>
#include <scene.h>

namespace Simply2D
{
	struct ApplicationSpecifications
	{
		std::string name = "Simply2D";
		std::string version = "1.0";
		std::string identifier = "com.terrytoupi.simply2D-empty";
		RendererSpecifications renderer;
		AssetDatabaseSpecifications assets;
	};

	class Application
	{
	public:
		static Application& GetInstance()
		{
			static Application instance;
			return instance;
		}
		std::shared_ptr<Renderer> GetRenderer()
		{
			return m_renderer;
		}
		std::shared_ptr<AssetDatabase> GetAssetDatabase()
		{
			return m_assetDatabase;
		}
		std::shared_ptr<SceneManager> GetSceneManager()
		{
			return m_sceneManager;
		}

		void create(const ApplicationSpecifications& specs = ApplicationSpecifications());
		void run();
		void stop();

		template<typename TLayer>
		requires(std::is_base_of_v<Layer, TLayer>)
		void pushLayer()
		{
			m_layers.push_back(std::make_unique<TLayer>());
		}

	private:
		Application() = default;
		~Application() = default;
		Application(const Application& app) = delete;
		Application& operator=(const Application&) = delete;

		bool m_running = false;

		ApplicationSpecifications m_specifications;
		std::shared_ptr<Renderer> m_renderer;
		std::shared_ptr<AssetDatabase> m_assetDatabase;
		std::shared_ptr<SceneManager> m_sceneManager;

		std::vector<std::unique_ptr<Layer>> m_layers;
	};
}