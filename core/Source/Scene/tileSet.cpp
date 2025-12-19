#include "Base/app.h"
#include "Scene/tileSet.h"
#include "Scene/tileLayers.h"

namespace Simply2D
{
	TileSet::TileSet(uint16_t tileWidth, uint16_t tileHeight, THandle<Image> setImage)
		:	m_tileWidth(tileWidth), m_tileHeight(tileHeight), m_imageHandle(setImage)
	{
		m_tileSetTexture = Application::GetRenderer()->createTexture(setImage);

		int width = 0, height = 0;
		Application::GetRenderer()->textureSize(m_tileSetTexture, width, height);

		m_tileSetWidth = width / m_tileWidth;
		m_tileSetHeight = height / m_tileHeight;

		for (uint16_t col = 0; col < m_tileSetHeight; ++col)
		{
			for (uint16_t row = 0; row < m_tileSetWidth; ++row)
			{
				uint16_t x = (row * m_tileWidth);
				uint16_t y = (col * m_tileHeight);
				m_tiles.push_back({x, y});
			}
		}
		
		return;
	}

	TileSet::~TileSet()
	{
		Application::GetRenderer()->destroyTexture(m_tileSetTexture);
		m_tiles.clear();
	}

	THandle<Texture> TileSet::texture()
	{
		return m_tileSetTexture;
	}

	Tile TileSet::getTile(uint16_t index)
	{
		return m_tiles.at(index);
	} 

	Tile TileSet::getTile(uint16_t x, uint16_t y)
	{
		return m_tiles.at(y * m_tileSetWidth + x);
	}

	uint16_t TileSet::getTileWidth()
	{
		return m_tileWidth;
	}

	uint16_t TileSet::getTileHeight()
	{
		return m_tileHeight;
	}

	uint16_t TileSet::getTileSetWidth()
	{
		return m_tileSetWidth;
	}

	uint16_t TileSet::getTileSetHeight()
	{
		return m_tileSetHeight;
	}
}