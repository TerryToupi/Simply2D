#pragma once

#include <assets.h>
#include <tileSet.h>
#include <tileLayers.h>

namespace Simply2D
{
	class Scene
	{
	public:
		Scene(std::string level);
		virtual ~Scene() = default;

		virtual void load()		{}
		virtual void update()	{} 
		virtual void render()	{}
		virtual void unload()	{}

	protected:
		std::shared_ptr<TileSet> m_tileset;
		std::vector<std::shared_ptr<TileLayer>> m_layers;
	};

	class SceneManager
	{
	public:
		SceneManager() = default;
		~SceneManager() = default;

		template<typename TScene>
		requires(std::is_base_of_v<Scene, TScene>)
		void pushScene(std::string level)
		{
			m_scenes.push_back(std::make_shared<TScene>(level));
		}

		void setActive(uint8_t index = 0);

		std::shared_ptr<Scene> get();

	private:
		static std::shared_ptr<SceneManager> Create();

		uint8_t m_activeIndex = 0;
		std::vector<std::shared_ptr<Scene>> m_scenes;

		friend class Application;
	};
}