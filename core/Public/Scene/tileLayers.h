#pragma once 

#include "tileSet.h"

#include "Rendering/renderer.h"
#include "Types/span.h"
#include "Types/SmartPointers.h"

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
		TileLayer(const TileLayerSpecifications& specs, Ref<TileSet> tilset);
		~TileLayer();

		void putTile(const int id = 0, const int x = 0, const int y = 0);
		THandle<Texture> texture();

		void filterScrollDistance(uint32_t viewStartCoord, uint32_t viewSize, uint32_t* d, uint32_t maxSize);

		uint32_t getPixelWidth() const { return m_specs.width * m_tileSet->getTileWidth(); }
		uint32_t getPixelHeight() const { return m_specs.height	* m_tileSet->getTileWidth(); }

	private:
		TileLayerSpecifications m_specs;
		Ref<TileSet> m_tileSet;
		THandle<Texture> m_texture;
	};
}