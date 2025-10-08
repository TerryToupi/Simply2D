#include <pch.h> 

#include <app.h>
#include <tileLayers.h>

namespace Simply2D
{
	TileLayer::TileLayer(const TileLayerSpecifications& specs, TileSet* tileSet)
		:	m_specs(specs), m_tileSet(tileSet)
	{
		m_texture = Application::GetInstance().GetRenderer()->createTexture({
			.format = TextureFormat::RGBA8_UNORM,
			.access = TextureAccess::READ_WRITE,
			.width = m_specs.width * tileSet->getTileWidth(),
			.height = m_specs.height * tileSet->getTileHeight(),
		});
	}

	TileLayer::~TileLayer()
	{
		Application::GetInstance().GetRenderer()->destroyTexture(m_texture);
	}

	void TileLayer::putTile(const int id, const int x, const int y)
	{
		if (id != 0)
		{
			Tile tile = m_tileSet->getTile(id - 1);
			uint16_t tileWidth = m_tileSet->getTileWidth();
			uint16_t tileHeight = m_tileSet->getTileHeight();
			Handle<Texture> tileset = m_tileSet->texture();

			Application::GetInstance().GetRenderer()->draw({
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

	Handle<Texture> TileLayer::texture()
	{
		return m_texture;
	}
}