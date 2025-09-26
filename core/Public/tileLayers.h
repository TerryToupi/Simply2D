#pragma once 

#include <array>

#include <span.h>
#include <tileSet.h>
#include <renderer.h>

namespace Simply2D 
{
	struct TileLayerSpecifications
	{
		int layerIndex = -1;
		int width = 0;
		int height = 0;
	};

	class TileLayer
	{
	public:
		TileLayer() = default;
		TileLayer(const TileLayerSpecifications& specs, std::shared_ptr<TileSet> tilset);
		~TileLayer();

		void putTile(const int id = 0, const int x = 0, const int y = 0);
		void generateTexture();
		Handle<Texture> texture();

	private:
		TileLayerSpecifications m_specs;
		std::shared_ptr<TileSet> m_tileSet;
		std::vector<DrawCall> m_calls;
		Handle<Texture> m_texture;
	};
}