#pragma once

#include <assets.h>

#include <string>
#include <unordered_map>

namespace Simply2D
{
	class SceneManager;

	class Scene
	{
	public:
		void load()		{}
		void activate() {}
		void update()	{} 
		void disable()	{}
		void unload()	{}

	protected:
		std::unordered_map<std::string, Asset> m_assets;
	};
}