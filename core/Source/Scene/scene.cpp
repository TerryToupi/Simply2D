#include <variant>

#include "Base/app.h"
#include "Scene/scene.h"
#include "Types/Utils.h"
#include "Types/Json.h"

#include "Source/Base/assetDataBase.h"
#include "Source/Systems/colisionChecker.h"

namespace Simply2D
{
	Scene::Scene(Asset level) 
	{
		AssetDatabaseImpl* assets = (AssetDatabaseImpl*)(Application::GetAssetDatabase());

		json* config = assets->getSerializable(THandle<Serializable>(level.handle));
		// Generating the tileset
		{
			uint16_t tileWidth = (uint16_t)(*config)["tilesets"][0]["tilewidth"];
			uint16_t tileHeight = (uint16_t)(*config)["tilesets"][0]["tileheight"];
	
			std::string imagePath = "/images";
			std::string configTilesetPath = (std::string)(*config)["tilesets"][0]["image"];
			std::size_t pos = configTilesetPath.find("images/");
			if (pos != std::string::npos) 
			{
				imagePath = configTilesetPath.substr(pos);
			}
			
			// if it's not load it ... well load it
			assets->load(AssetType::IMAGE, imagePath);
			m_tileset = MakeRef<TileSet>(tileWidth, tileHeight, assets->get(imagePath));
		}

		// reserviing the size of the layer vector
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
	}
}
