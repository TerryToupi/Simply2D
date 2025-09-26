#include <pch.h>

#include <app.h>
#include <scene.h>

#include <Source/assetDataBase.h>

#include <nlohmann/json.hpp>
using json = nlohmann::json;

namespace Simply2D
{
	void Scene::generateTileLayers(Asset sceneAsset)
	{
		AssetDatabaseImpl* assets = (AssetDatabaseImpl*)(Application::GetInstance().GetAssetDatabase().get());

		json* config = assets->getSerializable(Handle<Serializable>(sceneAsset.handle));
		
		// Generating the tileset
		{
			uint16_t tileWidth = (*config)["tilesets"][0]["tilewidth"];
			uint16_t tileHeight = (*config)["tilesets"][0]["tileheight"];
	
			std::string imagePath = "/images";
			std::string configTilesetPath = (*config)["tilesets"][0]["image"];
			std::size_t pos = configTilesetPath.find("images/");
			if (pos != std::string::npos) 
			{
				imagePath = configTilesetPath.substr(pos);
			}
			
			// if it's not load it ... well load it
			assets->load(AssetType::IMAGE, imagePath);

			m_tileset = std::make_shared<TileSet>(tileWidth, tileHeight, assets->get(imagePath));
		}

		// reserviing the size of the layer vector
		uint16_t reserveSize = (*config)["layers"].size();
		m_layers.reserve(reserveSize);
	
		// Generating the Layers
		for (unsigned i = 0; i < reserveSize; ++i)
		{
			TileLayerSpecifications specs
			{
				.layerIndex = (*config)["layers"][i]["id"],
				.width = (*config)["layers"][i]["width"],
				.height = (*config)["layers"][i]["height"]
			};
			m_layers.push_back(std::make_shared<TileLayer>(specs, m_tileset));

			for (unsigned h = 0; h < (*config)["layers"][i]["height"]; ++h)
			{
				for (unsigned w = 0; w < (*config)["layers"][i]["width"]; ++w)
				{
					uint16_t index = h * (*config)["layers"][i]["width"] + w;
					uint16_t tileID = (*config)["layers"][i]["data"][index];
					m_layers.at(i)->putTile(tileID, w, h);
				}
			}

			m_layers.at(i)->generateTexture();
		}
	}
}
