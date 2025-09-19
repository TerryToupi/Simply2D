#pragma once

#include <string>
#include <vector>
#include <layer.h>
#include <renderer.h>

namespace core
{
	struct ApplicationSpecifications
	{
		std::string name = "Simply2D";
		std::string version = "1.0";
		std::string identifier = "com.terrytoupi.simply2D-empty";
		RendererSpecifications renderer;
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

		void create(const ApplicationSpecifications& specs = ApplicationSpecifications());
		void run();
		void stop();
		void destroy();

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
		std::vector<std::unique_ptr<Layer>> m_layers;
	};

	static inline Application& app()
	{
		return Application::GetInstance();
	}

	static inline Renderer& gfx()
	{
		return *Application::GetInstance().GetRenderer();
	}
}