#include "Base/app.h"
#include "Scene/tileLayers.h"

namespace Simply2D
{
	TileLayer::TileLayer(const TileLayerSpecifications& specs, Ref<TileSet> tileSet)
		:	m_specs(specs), m_tileSet(tileSet)
	{
		m_texture = Application::GetRenderer()->createTexture({
			.format = TextureFormat::RGBA8_UNORM,
			.access = TextureAccess::READ_WRITE,
			.width = m_specs.width * tileSet->getTileWidth(),
			.height = m_specs.height * tileSet->getTileHeight(),
		});
	}

	TileLayer::~TileLayer()
	{
		Application::GetRenderer()->destroyTexture(m_texture);
	}

	void TileLayer::putTile(const int id, const int x, const int y)
	{
		if (id != 0)
		{
			Tile tile = m_tileSet->getTile(id - 1);
			uint16_t tileWidth = m_tileSet->getTileWidth();
			uint16_t tileHeight = m_tileSet->getTileHeight();
			THandle<Texture> tileset = m_tileSet->texture();

			Application::GetRenderer()->draw({
				.target = m_texture, 
				.loadOp = LoadOp::LOAD, 
				.storeOp = StoreOp::STORE
				},
				{
					{
						.texture = tileset,
						.blend = Blend::BLEND,
						.src = {tile.x, tile.y, tileWidth, tileHeight},
						.dist = {x * tileWidth, y * tileHeight, tileWidth, tileHeight}
					}
				}
				);
		}
	}

	THandle<Texture> TileLayer::texture()
	{
		return m_texture;
	}

	void TileLayer::filterScrollDistance(uint32_t viewStartCoord, uint32_t viewSize, uint32_t* d, uint32_t maxSize)
	{
		auto val = *d + viewStartCoord;
		if (val < 0)
			*d = viewStartCoord;
		else if (viewSize >= maxSize)
			*d = 0;
		else if ((val + viewSize) >= maxSize)
			*d = maxSize - (viewStartCoord + viewSize);
	}

	void TileLayer::filterScrollDistanceX(uint32_t viewStartCoord, uint32_t viewSize, uint32_t* d)
	{
		filterScrollDistance(viewStartCoord, viewSize, d, getPixelWidth());
	}

	void TileLayer::filterScrollDistanceY(uint32_t viewStartCoord, uint32_t viewSize, uint32_t* d)
	{
		filterScrollDistance(viewStartCoord, viewSize, d, getPixelHeight());
	}
}