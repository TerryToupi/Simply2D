#include <pch.h>

#include <app.h>
#include <scene.h>
#include <sprites.h>

#include <Source/assetDataBase.h>

#include <nlohmann/json.hpp>
using json = nlohmann::json;

namespace Simply2D
{
	Scene::Scene(std::string level)
	{
		AssetDatabaseImpl* assets = (AssetDatabaseImpl*)(Application::GetInstance().GetAssetDatabase().get());

		json* config = assets->getSerializable(Handle<Serializable>(assets->get(level).handle));
		
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
				.layerIndex = (uint16_t)(*config)["layers"][i]["id"],
				.width = (uint16_t)(*config)["layers"][i]["width"],
				.height = (uint16_t)(*config)["layers"][i]["height"]
			};
			m_layers.push_back(std::make_shared<TileLayer>(specs, m_tileset));

			for (unsigned h = 0; h < (*config)["layers"][i]["height"]; ++h)
			{
				for (unsigned w = 0; w < (*config)["layers"][i]["width"]; ++w)
				{
					uint16_t index = h * (uint16_t)(*config)["layers"][i]["width"] + w;
					uint16_t tileID = (uint16_t)(*config)["layers"][i]["data"][index];
					m_layers.at(i)->putTile(tileID, w, h);
				}
			}

			m_layers.at(i)->generateTexture();
		}
	}

	Sprite Scene::createSprite(std::string name)
	{
		Sprite sprite = { m_spritesRegistry.create(), this };
		auto& tag = sprite.addComponent<Tag>();
		tag.tag = name;

		return sprite;
	}

	Sprite Scene::getSprite(std::string_view name)
	{
		auto view = m_spritesRegistry.view<Tag>();
		for (auto entity : view)
		{
			const auto& tag = view.get<Tag>(entity);
			if (tag.tag == name)
				return { entity, this };
		}

		return {};
	}

	void SceneManager::setActive(uint8_t index)
	{
		m_activeIndex = index;
	}

	std::shared_ptr<Scene> SceneManager::get()
	{
		return m_scenes.at(m_activeIndex);
	}

	std::shared_ptr<SceneManager> SceneManager::Create()
	{
		return std::make_shared<SceneManager>();
	}
}
