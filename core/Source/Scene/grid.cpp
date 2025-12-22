#include "Scene/grid.h"
#include "Source/Base/assetDataBase.h"
#include "Source/Rendering/renderingBackend.h"
#include "Base/app.h"
#include "Rendering/renderer.h"

#include <SDL3/SDL.h>
#include <iostream>

namespace Simply2D
{
	// ============== TileColorsHolder ==============

	void TileColorsHolder::insertTileColors(THandle<Image> image, AssetDatabaseImpl* assets,
		const Tile& tile, uint16_t tileWidth, uint16_t tileHeight, uint16_t tileIndex)
	{
		if (m_processedIndices.find(tileIndex) != m_processedIndices.end())
			return;

		m_processedIndices.insert(tileIndex);

		for (uint16_t py = 0; py < tileHeight; ++py)
		{
			for (uint16_t px = 0; px < tileWidth; ++px)
			{
				int pixelX = tile.x + px;
				int pixelY = tile.y + py;

				if (!assets->isPixelTransparent(image, pixelX, pixelY))
				{
					uint32_t pixel = assets->getPixelAt(image, pixelX, pixelY);
					m_emptyColors.insert(pixel);
				}
			}
		}
	}

	bool TileColorsHolder::isEmptyColor(uint32_t color) const
	{
		return m_emptyColors.find(color) != m_emptyColors.end();
	}

	// ============== GridLayer ==============

	GridLayer::GridLayer(uint16_t mapWidth, uint16_t mapHeight, const GridConfig& config)
		: m_config(config), m_mapWidth(mapWidth), m_mapHeight(mapHeight)
	{
		size_t totalElements = static_cast<size_t>(mapWidth) * config.gridBlockColumns
			* static_cast<size_t>(mapHeight) * config.gridBlockRows;
		m_grid.resize(totalElements, GRID_EMPTY);
	}

	GridLayer::~GridLayer()
	{
		if (m_debugTexturesInitialized)
		{
			Renderer* renderer = Application::GetRenderer();
			renderer->destroyTexture(m_debugSolidTexture);
			renderer->destroyTexture(m_debugEmptyTexture);
		}
	}

	bool GridLayer::isSolid(int pixelX, int pixelY) const
	{
		if (pixelX < 0 || pixelY < 0)
			return false;

		uint16_t gridCol = static_cast<uint16_t>(pixelX / m_config.gridElementWidth());
		uint16_t gridRow = static_cast<uint16_t>(pixelY / m_config.gridElementHeight());

		if (gridCol >= gridWidth() || gridRow >= gridHeight())
			return false;

		return getElement(gridCol, gridRow) == GRID_SOLID;
	}

	GridIndex GridLayer::getElement(uint16_t gridCol, uint16_t gridRow) const
	{
		if (gridCol >= gridWidth() || gridRow >= gridHeight())
			return GRID_EMPTY;

		size_t idx = static_cast<size_t>(gridRow) * gridWidth() + gridCol;
		return m_grid[idx];
	}

	void GridLayer::setElement(uint16_t gridCol, uint16_t gridRow, GridIndex value)
	{
		if (gridCol >= gridWidth() || gridRow >= gridHeight())
			return;

		size_t idx = static_cast<size_t>(gridRow) * gridWidth() + gridCol;
		m_grid[idx] = value;
	}

	void GridLayer::setTileBlock(uint16_t tileCol, uint16_t tileRow, GridIndex value)
	{
		if (tileCol >= m_mapWidth || tileRow >= m_mapHeight)
			return;

		uint16_t startGridCol = tileCol * m_config.gridBlockColumns;
		uint16_t startGridRow = tileRow * m_config.gridBlockRows;

		for (uint16_t r = 0; r < m_config.gridBlockRows; ++r)
		{
			for (uint16_t c = 0; c < m_config.gridBlockColumns; ++c)
			{
				setElement(startGridCol + c, startGridRow + r, value);
			}
		}
	}

	void GridLayer::initDebugTextures()
	{
		if (m_debugTexturesInitialized)
			return;

		auto* rendererImpl = static_cast<RendererImpl*>(Application::GetRenderer());
		SDL_Renderer* sdlRenderer = rendererImpl->m_rendererHandle;

		// Create 1x1 red surface for solid cells
		SDL_Surface* solidSurface = SDL_CreateSurface(1, 1, SDL_PIXELFORMAT_RGBA8888);
		if (solidSurface)
		{
			const SDL_PixelFormatDetails* fmt = SDL_GetPixelFormatDetails(solidSurface->format);
			uint32_t red = SDL_MapRGBA(fmt, nullptr, 255, 0, 0, 255);
			SDL_FillSurfaceRect(solidSurface, nullptr, red);

			SDL_Texture* solidTex = SDL_CreateTextureFromSurface(sdlRenderer, solidSurface);
			SDL_DestroySurface(solidSurface);

			if (solidTex)
				m_debugSolidTexture = rendererImpl->m_textures.Insert(std::move(solidTex));
		}

		// Create 1x1 green surface for empty cells
		SDL_Surface* emptySurface = SDL_CreateSurface(1, 1, SDL_PIXELFORMAT_RGBA8888);
		if (emptySurface)
		{
			const SDL_PixelFormatDetails* fmt = SDL_GetPixelFormatDetails(emptySurface->format);
			uint32_t green = SDL_MapRGBA(fmt, nullptr, 0, 255, 0, 255);
			SDL_FillSurfaceRect(emptySurface, nullptr, green);

			SDL_Texture* emptyTex = SDL_CreateTextureFromSurface(sdlRenderer, emptySurface);
			SDL_DestroySurface(emptySurface);

			if (emptyTex)
				m_debugEmptyTexture = rendererImpl->m_textures.Insert(std::move(emptyTex));
		}

		m_debugTexturesInitialized = true;
	}

	void GridLayer::debugRender(int offsetX, int offsetY, int targetWidth, int targetHeight)
	{
		initDebugTextures();

		Renderer* renderer = Application::GetRenderer();
		TVector<DrawCall> calls;

		uint16_t elemWidth = m_config.gridElementWidth();
		uint16_t elemHeight = m_config.gridElementHeight();

		// Calculate map dimensions in pixels
		int mapPixelWidth = m_mapWidth * m_config.tileWidth;
		int mapPixelHeight = m_mapHeight * m_config.tileHeight;

		// Calculate scale factors if target dimensions are specified
		float scaleX = (targetWidth > 0) ? (static_cast<float>(targetWidth) / mapPixelWidth) : 1.0f;
		float scaleY = (targetHeight > 0) ? (static_cast<float>(targetHeight) / mapPixelHeight) : 1.0f;

		for (uint16_t row = 0; row < gridHeight(); ++row)
		{
			for (uint16_t col = 0; col < gridWidth(); ++col)
			{
				GridIndex elem = getElement(col, row);

				DrawCall call;
				call.texture = (elem == GRID_SOLID) ? m_debugSolidTexture : m_debugEmptyTexture;

				// Calculate start and end positions to avoid rounding gaps
				int x1 = offsetX + static_cast<int>(col * elemWidth * scaleX);
				int y1 = offsetY + static_cast<int>(row * elemHeight * scaleY);
				int x2 = offsetX + static_cast<int>((col + 1) * elemWidth * scaleX);
				int y2 = offsetY + static_cast<int>((row + 1) * elemHeight * scaleY);

				call.dist = {x1, y1, x2 - x1, y2 - y1};
				call.src = {0, 0, 1, 1};

				// Alpha: semi-transparent red for solid, more transparent green for empty
				call.alpha = (elem == GRID_SOLID) ? 120 : 60;
				call.blend = Blend::BLEND;

				calls.push_back(call);
			}
		}

		RenderDescriptor desc;
		desc.target = SURFACE;
		desc.loadOp = LoadOp::LOAD;
		desc.storeOp = StoreOp::STORE;

		renderer->draw(desc, Span(calls.data(), calls.size()));
	}

	// ============== ComputeGrid ==============

	void ComputeGrid(
		GridLayer& grid,
		const TVector<uint16_t>& tileData,
		uint16_t layerWidth,
		uint16_t layerHeight,
		Ref<TileSet> tileSet,
		THandle<Image> tilesetImage,
		AssetDatabaseImpl* assets,
		const TSet<uint16_t>& emptyTileIndices)
	{
		int imageWidth = 0, imageHeight = 0;
		assets->getImageSize(tilesetImage, imageWidth, imageHeight);
		if (imageWidth == 0 || imageHeight == 0)
			return;

		const GridConfig& cfg = grid.config();
		TileColorsHolder emptyColors;

		// Debug: Count how many tiles are marked as empty
		int emptyTileCount = 0;
		TSet<uint16_t> uniqueTileIds;

		for (uint16_t tileRow = 0; tileRow < layerHeight; ++tileRow)
		{
			for (uint16_t tileCol = 0; tileCol < layerWidth; ++tileCol)
			{
				size_t tileIdx = static_cast<size_t>(tileRow) * layerWidth + tileCol;
				uint16_t tileId = tileData[tileIdx];

				// Track unique tile IDs
				uniqueTileIds.insert(tileId);

				// Tile ID 0 = no tile, treat as empty
				if (tileId == 0)
				{
					grid.setTileBlock(tileCol, tileRow, GRID_EMPTY);
					continue;
				}

				// Check if tile is in empty set
				if (emptyTileIndices.find(tileId) != emptyTileIndices.end())
				{
					grid.setTileBlock(tileCol, tileRow, GRID_EMPTY);
					++emptyTileCount;

					// Collect colors from this empty tile (slide 3)
					Tile tile = tileSet->getTile(tileId - 1);
					emptyColors.insertTileColors(tilesetImage, assets, tile,
						cfg.tileWidth, cfg.tileHeight, tileId);
					continue;
				}

				// Get tile position in tileset
				Tile tile = tileSet->getTile(tileId - 1);

				// Analyze each grid element (4x4 = 16 elements per tile)
				for (uint16_t elemIdx = 0; elemIdx < cfg.elementsPerTile(); ++elemIdx)
				{
					uint16_t elemX = (elemIdx % cfg.gridBlockColumns) * cfg.gridElementWidth();
					uint16_t elemY = (elemIdx / cfg.gridBlockColumns) * cfg.gridElementHeight();

					int solidCount = 0;

					// Count solid pixels in this grid element based on detection mode
					for (uint16_t py = 0; py < cfg.gridElementHeight(); ++py)
					{
						for (uint16_t px = 0; px < cfg.gridElementWidth(); ++px)
						{
							int pixelX = tile.x + elemX + px;
							int pixelY = tile.y + elemY + py;

							bool isEmptyPixel = false;

							switch (cfg.detectionMode)
							{
							case GridDetectionMode::Alpha:
								isEmptyPixel = assets->isPixelTransparent(tilesetImage, pixelX, pixelY);
								break;
							case GridDetectionMode::Brightness:
								isEmptyPixel = assets->getPixelBrightness(tilesetImage, pixelX, pixelY) > cfg.brightnessThreshold;
								break;
							case GridDetectionMode::Both:
								isEmptyPixel = assets->isPixelTransparent(tilesetImage, pixelX, pixelY) ||
									assets->getPixelBrightness(tilesetImage, pixelX, pixelY) > cfg.brightnessThreshold;
								break;
							}

							if (!isEmptyPixel)
							{
								++solidCount;
							}
						}
					}

					// Calculate grid position
					uint16_t gridCol = tileCol * cfg.gridBlockColumns + (elemIdx % cfg.gridBlockColumns);
					uint16_t gridRow = tileRow * cfg.gridBlockRows + (elemIdx / cfg.gridBlockColumns);

					// Use ratio-based threshold: solid if (solidCount / totalPixels) > threshold
					float ratio = static_cast<float>(solidCount) / cfg.pixelsPerElement();
					grid.setElement(gridCol, gridRow,
						ratio > cfg.solidRatioThreshold ? GRID_SOLID : GRID_EMPTY);
				}
			}
		}

		// Debug output
		std::cout << "ComputeGrid Debug:" << std::endl;
		std::cout << "  Empty tile indices provided: " << emptyTileIndices.size() << std::endl;
		std::cout << "  Tiles marked as empty in map: " << emptyTileCount << std::endl;
		std::cout << "  Unique tile IDs in map: ";
		for (auto id : uniqueTileIds)
			std::cout << id << " ";
		std::cout << std::endl;
		std::cout << "  Tile IDs from map that matched empty list: ";
		for (auto id : uniqueTileIds)
			if (emptyTileIndices.find(id) != emptyTileIndices.end())
				std::cout << id << " ";
		std::cout << std::endl;
	}
}
