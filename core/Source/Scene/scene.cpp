#include <variant>

#include "Base/app.h"
#include "Scene/scene.h"

#include <Source/Base/assetDataBase.h>
#include <Source/Systems/colisionChecker.h>

#include <nlohmann/json.hpp>
using json = nlohmann::json;

namespace Simply2D
{
	Scene::Scene(Asset level) 
	{
		AssetDatabaseImpl* assets = (AssetDatabaseImpl*)(Application::GetInstance()->GetAssetDatabase());

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
			m_tileset.emplace(tileWidth, tileHeight, assets->get(imagePath));
		}

		// reserviing the size of the layer vector
		m_layersCount = (*config)["layers"].size();
	
		// Generating the Layers
		for (unsigned i = 0; i < m_layersCount; ++i)
		{
			TileLayerSpecifications specs
			{
				.layerIndex = (uint16_t)(*config)["layers"][i]["id"],
				.width = (uint16_t)(*config)["layers"][i]["width"],
				.height = (uint16_t)(*config)["layers"][i]["height"]
			};
			m_layers[i].emplace(specs, &m_tileset.value());

			for (unsigned h = 0; h < (*config)["layers"][i]["height"]; ++h)
			{
				for (unsigned w = 0; w < (*config)["layers"][i]["width"]; ++w)
				{
					uint16_t index = h * (uint16_t)(*config)["layers"][i]["width"] + w;
					uint16_t tileID = (uint16_t)(*config)["layers"][i]["data"][index];
					m_layers.at(i)->putTile(tileID, w, h);
				}
			}
		}
	}

	void Scene::registerSprite(Sprite* sprite)
	{
		m_spriteRegister.push_back(sprite);
	}
}
