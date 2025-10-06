#pragma once

#include <assets.h>
#include <tileSet.h>
#include <tileLayers.h>

#include <unordered_map>

namespace Simply2D
{
	class Sprite;
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

	public:
		// API functions
		Sprite* createSprite(std::string name);
		Sprite* getSprite(std::string name);

	protected:
		SceneManager*							m_manager;
		std::shared_ptr<TileSet>				m_tileset;
		std::vector<std::shared_ptr<TileLayer>> m_layers;
		
		std::unordered_map<std::string, Sprite*> m_sprites;

		friend class Sprite;
		friend class SceneManager;
	};
}