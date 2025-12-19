#pragma once

#include "Types/Arrays.h"
#include "Types/Set.h"
#include "Types/SmartPointers.h"
#include "Base/resources.h"
#include "tileSet.h"
#include <cstdint>

namespace Simply2D
{
	class AssetDatabaseImpl;

	using GridIndex = uint8_t;
	constexpr GridIndex GRID_EMPTY = 0;
	constexpr GridIndex GRID_SOLID = 1;

	struct GridSetupDescriptor {
		TVector<uint16_t> tileData;
		uint16_t mapWidth = 0;
		uint16_t mapHeight = 0;
		int layerIndex = -1;
	};

	struct GridConfig
	{
		uint16_t tileWidth = 32;
		uint16_t tileHeight = 32;
		uint16_t gridBlockColumns = 4;
		uint16_t gridBlockRows = 4;

		uint16_t gridElementWidth() const { return tileWidth / gridBlockColumns; }
		uint16_t gridElementHeight() const { return tileHeight / gridBlockRows; }
		uint16_t elementsPerTile() const { return gridBlockColumns * gridBlockRows; }
	};

	class TileColorsHolder
	{
	public:
		void insertTileColors(THandle<Image> image, AssetDatabaseImpl* assets,
			const Tile& tile, uint16_t tileWidth, uint16_t tileHeight, uint16_t tileIndex);

		bool isEmptyColor(uint32_t color) const;

	private:
		TSet<uint16_t> m_processedIndices;
		TSet<uint32_t> m_emptyColors;
	};

	class GridLayer
	{
	public:
		GridLayer() = default;
		GridLayer(uint16_t mapWidth, uint16_t mapHeight, const GridConfig& config);
		~GridLayer();

		bool isSolid(int pixelX, int pixelY) const;

		GridIndex getElement(uint16_t gridCol, uint16_t gridRow) const;
		void setElement(uint16_t gridCol, uint16_t gridRow, GridIndex value);

		void setTileBlock(uint16_t tileCol, uint16_t tileRow, GridIndex value);

		// Debug visualization
		// targetWidth/targetHeight specify the rendering dimensions (for scaling to window size)
		// If 0, uses actual pixel dimensions (no scaling)
		void debugRender(int offsetX = 0, int offsetY = 0, int targetWidth = 0, int targetHeight = 0);

		uint16_t gridWidth() const { return m_mapWidth * m_config.gridBlockColumns; }
		uint16_t gridHeight() const { return m_mapHeight * m_config.gridBlockRows; }
		uint16_t mapWidth() const { return m_mapWidth; }
		uint16_t mapHeight() const { return m_mapHeight; }
		const GridConfig& config() const { return m_config; }

	private:
		void initDebugTextures();

		GridConfig m_config;
		uint16_t m_mapWidth = 0;
		uint16_t m_mapHeight = 0;
		TVector<GridIndex> m_grid;

		// Debug textures (lazy initialized)
		THandle<Texture> m_debugSolidTexture;
		THandle<Texture> m_debugEmptyTexture;
		bool m_debugTexturesInitialized = false;
	};

	void ComputeGrid(
		GridLayer& grid,
		const TVector<uint16_t>& tileData,
		uint16_t layerWidth,
		uint16_t layerHeight,
		Ref<TileSet> tileSet,
		THandle<Image> tilesetImage,
		AssetDatabaseImpl* assets,
		const TSet<uint16_t>& emptyTileIndices,
		uint8_t solidThreshold = 0
	);
}
