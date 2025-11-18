#pragma once

#include <string>
#include <vector>
#include <functional>

#include "layer.h"
#include "assets.h"

#include "Rendering/renderer.h"
#include "Scene/scene.h"
#include "Types/mtRingBuffer.h"
#include "Memory/memoryAllocator.h"

namespace Simply2D
{
	struct ApplicationSpecifications
	{
		std::string name = "Simply2D";
		std::string version = "1.0";
		std::string identifier = "com.terrytoupi.simply2D-empty";
		AllocatorSpecifications memory;
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

		void create(const ApplicationSpecifications& specs = ApplicationSpecifications());
		void run();
		void stop();
		void destroy();

		template<typename TLayer>
		requires(std::is_base_of_v<Layer, TLayer>)
		void pushLayer()
		{
			m_layers.push_back(std::make_shared<TLayer>());
		}

	private:
		Application() = default;
		~Application() = default;
		Application(const Application& app) = delete;
		Application& operator=(const Application&) = delete;

	private:
		bool m_running = false;

		ApplicationSpecifications m_specifications;
		std::shared_ptr<Renderer> m_renderer;
		std::shared_ptr<AssetDatabase> m_assetDatabase;

		std::vector<std::shared_ptr<Layer>> m_layers;

		MTRingQueue<std::function<void()>, 256> m_mainThreadQueue;
	
		friend class Layer;
	};
}