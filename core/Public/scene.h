#pragma once

#include <assets.h>
#include <tileSet.h>
#include <tileLayers.h>

namespace Simply2D
{
	class SceneManager;

	class Scene
	{
	public:
		virtual void load()		{}
		virtual void update()	{} 
		virtual void render()	{}
		virtual void unload()	{}

	protected:
		void generateTileLayers(Asset sceneAsset);

	protected:
		std::shared_ptr<TileSet> m_tileset;
		std::vector<std::shared_ptr<TileLayer>> m_layers;
	};
}