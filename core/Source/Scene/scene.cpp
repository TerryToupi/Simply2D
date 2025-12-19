#include <variant>

#include "Base/app.h"
#include "Scene/scene.h"
#include "Scene/grid.h"
#include "Types/Utils.h"
#include "Types/Json.h"

#include "Source/Base/assetDataBase.h"

namespace Simply2D
{
	Scene::Scene(const std::string& vfp) 
	{
		auto* assets = static_cast<AssetDatabaseImpl*>(Application::GetAssetDatabase());
		
		THandle<Json> levelHandle = assets->load<Json>(vfp);
		json* config = assets->getSerializable(levelHandle);
		// Generating the tileset
		{
			auto tileWidth = static_cast<uint16_t>((*config)["tilesets"][0]["tilewidth"]);
			auto tileHeight = static_cast<uint16_t>((*config)["tilesets"][0]["tileheight"]);
			auto name = static_cast<std::string>((*config)["tilesets"][0]["name"]);

			THandle<Image> image = assets->load<Image>("image://" + name);
			m_tileset = MakeRef<TileSet>(tileWidth, tileHeight, image);
		}

		// reserving the size of the layer vector
		m_layersCount = (*config)["layers"].size();
		m_layers.reserve(m_layersCount);
		for (unsigned i = 0; i < m_layersCount; ++i)
		{
			TileLayerSpecifications specs
			{
				.layerIndex = (uint16_t)(*config)["layers"][i]["id"],
				.width = (uint16_t)(*config)["layers"][i]["width"],
				.height = (uint16_t)(*config)["layers"][i]["height"]
			};
			m_layers.emplace_back(specs, m_tileset);

			for (unsigned h = 0; h < (*config)["layers"][i]["height"]; ++h)
			{
				for (unsigned w = 0; w < (*config)["layers"][i]["width"]; ++w)
				{
					uint16_t index = h * (uint16_t)(*config)["layers"][i]["width"] + w;
					uint16_t tileID = (uint16_t)(*config)["layers"][i]["data"][index];
					m_layers.at(i).putTile(tileID, w, h);
				}
			}
		}

		// Store grid setup data for later computation (can't compute now because virtual dispatch doesn't work in constructor)
		if (m_layersCount > 0 && m_tileset)
		{
			m_gridSetup = MakeScope<GridSetupDescriptor>();

			// Find the first visible layer to use for collision grid
			int gridLayerIndex = -1;
			for (unsigned i = 0; i < m_layersCount; ++i)
			{
				// Safely check if layer is visible (default to true if field doesn't exist)
				bool isVisible = true;
				auto& layer = (*config)["layers"][i];
				auto it = layer.find("visible");
				if (it != layer.end())
				{
					isVisible = it->get<bool>();
				}

				if (isVisible)
				{
					gridLayerIndex = i;
					break;
				}
			}

			// Fall back to first layer if no visible layer found
			if (gridLayerIndex == -1)
				gridLayerIndex = 0;

			m_gridSetup->layerIndex = gridLayerIndex;
			m_gridSetup->mapWidth = static_cast<uint16_t>((*config)["layers"][gridLayerIndex]["width"]);
			m_gridSetup->mapHeight = static_cast<uint16_t>((*config)["layers"][gridLayerIndex]["height"]);

			// Extract tile data from the selected layer
			m_gridSetup->tileData.reserve(static_cast<size_t>(m_gridSetup->mapWidth) * m_gridSetup->mapHeight);
			for (unsigned h = 0; h < m_gridSetup->mapHeight; ++h)
			{
				for (unsigned w = 0; w < m_gridSetup->mapWidth; ++w)
				{
					uint16_t index = h * m_gridSetup->mapWidth + w;
					uint16_t tileID = static_cast<uint16_t>((*config)["layers"][gridLayerIndex]["data"][index]);
					m_gridSetup->tileData.push_back(tileID);
				}
			}

			// Create the empty grid structure
			GridConfig gridConfig;
			gridConfig.tileWidth = m_tileset->getTileWidth();
			gridConfig.tileHeight = m_tileset->getTileHeight();
			m_grid = MakeScope<GridLayer>(m_gridSetup->mapWidth, m_gridSetup->mapHeight, gridConfig);
		}
	}

	void Scene::computeGridIfNeeded()
	{
		if (m_gridComputed || !m_gridSetup || !m_grid)
			return;

		auto* assets = static_cast<AssetDatabaseImpl*>(Application::GetAssetDatabase());

		// Now we can safely call getEmptyTileIndices() because the derived class is fully constructed
		TSet<uint16_t> emptyTileIndices = getEmptyTileIndices();

		ComputeGrid(*m_grid, m_gridSetup->tileData, m_gridSetup->mapWidth, m_gridSetup->mapHeight,
			m_tileset, m_tileset->imageHandle(), assets, emptyTileIndices);

		m_gridComputed = true;
		m_gridSetup.reset();  // Free the setup data, no longer needed
	}
}
