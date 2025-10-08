#pragma once

#include <optional>

#include <assets.h>
#include <tileSet.h>
#include <tileLayers.h>

namespace Simply2D
{
	class Scene;

	class SceneManager
	{
	public:
		SceneManager() = default;
		~SceneManager() = default;

		void event();
		void update(float ts);
		void render();
		
		template<std::derived_from<Scene> TScene>
		void push(std::string level)
		{
			m_scenes.push_back(std::make_shared<TScene>(level, this));
		} 

	private:
		uint8_t m_activeIndex = 0;
		std::vector<std::shared_ptr<Scene>> m_scenes;

		friend class Scene;
	};

	class Scene
	{
	public:
		Scene(std::string level, SceneManager* manager);
		virtual ~Scene() = default;

		// Scripting functions
		virtual void event()			{}
		virtual void update(float ts)	{} 
		virtual void render()			{}

		template<typename TScene>
		requires(std::is_base_of_v<Scene, TScene>)
		void transition()
		{
			for (unsigned i = 0; i < m_manager->m_scenes.size(); ++i)
			{
				if (auto casted = dynamic_cast<TScene*>(m_manager->m_scenes.at(i).get()))
				{
					m_manager->m_activeIndex = i;
					return;
				}
			}
		}

	protected:
		std::optional<TileSet>					m_tileset;
		std::array<std::optional<TileLayer>, 6> m_layers;
		unsigned								m_layersCount = 0;

	private:
		SceneManager* m_manager;

		friend class SceneManager;
	};
}