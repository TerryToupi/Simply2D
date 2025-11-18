#pragma once 

#include <array>

#include "tileSet.h"

#include "Rendering/renderer.h"
#include "Types/span.h"

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
		TileLayer(const TileLayerSpecifications& specs, TileSet* tilset);
		~TileLayer();

		void putTile(const int id = 0, const int x = 0, const int y = 0);
		THandle<Texture> texture();

	private:
		TileLayerSpecifications m_specs;
		TileSet* m_tileSet;
		THandle<Texture> m_texture;
	};
}