#pragma once 

#include <array>

#include <span.h>
#include <tileSet.h>

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

	private:
		TileLayerSpecifications m_specs;
		std::shared_ptr<TileSet> m_tileSet;
		Handle<Texture> m_texture;
	};
}