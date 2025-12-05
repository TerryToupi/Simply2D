#include <variant>

#include "Base/app.h"
#include "Scene/scene.h"
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
