#include <pch.h>

#include <tileSet.h>
#include <app.h>
#include "tileLayers.h"

namespace Simply2D
{
	TileSet::TileSet(uint16_t tileWidth, uint16_t tileHeight, Asset setImage)
		:	m_tileWidth(tileWidth), m_tileHeight(tileHeight)
	{
		m_tileSetTexture = Application::GetInstance().GetRenderer()->createTexture(Handle<Image>(setImage.handle));

		int width = 0, height = 0;
		Application::GetInstance().GetRenderer()->textureSize(m_tileSetTexture, width, height);

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
		Application::GetInstance().GetRenderer()->destroyTexture(m_tileSetTexture);
		m_tiles.clear();
	}

	Handle<Texture> TileSet::texture()
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