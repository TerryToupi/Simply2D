#include <pch.h>

#include <variant>

#include <app.h>
#include <scene.h>
#include <TinyThreadPool.h>

#include <Source/assetDataBase.h>

#include <nlohmann/json.hpp>
using json = nlohmann::json;

namespace Simply2D
{
	Scene::Scene(Asset level, SceneManager* manager)
		:	m_manager(manager)
	{
		AssetDatabaseImpl* assets = (AssetDatabaseImpl*)(Application::GetInstance().GetAssetDatabase().get());

		json* config = assets->getSerializable(Handle<Serializable>(level.handle));
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
 
	void SceneManager::begin(float ts)
	{
		m_scenes.at(m_activeIndex)->begin(ts);
	}

	void SceneManager::event()
	{
		m_scenes.at(m_activeIndex)->event();
	}

	void SceneManager::update(float ts)
	{ 
		const auto& spriteRegister = m_scenes.at(m_activeIndex)->GetSpriteRegister();
		if (!spriteRegister.empty())
		{
			for (unsigned i = 0; i < spriteRegister.size(); ++i)
			{
				for (unsigned j = i + 1; j < spriteRegister.size(); ++j)
				{
					if (!spriteRegister[i]->GetBoundingArea().has_value() ||
						!spriteRegister[j]->GetBoundingArea().has_value())
						continue;

					TinyThreadPool::Execute([&i, &j, &spriteRegister]() 
					{
						if (auto* b1 = std::get_if<BoundingBox>(&spriteRegister[i]->GetBoundingArea().value()))
						{
							if (auto* b2 = std::get_if<BoundingBox>(&spriteRegister[j]->GetBoundingArea().value()))
							{
								b1->Intersects(*b2);
							}
						}
					});
				}
			}

			TinyThreadPool::Wait();
		}

		m_scenes.at(m_activeIndex)->update(ts);
	}

	void SceneManager::render()
	{
		m_scenes.at(m_activeIndex)->render();
	}

	void SceneManager::end(float ts)
	{
		m_scenes.at(m_activeIndex)->end(ts);
	}
}
