#pragma once

#include <Simly2D.h>

#include <string>
#include <iostream>
#include <unordered_map>

#include <level1.h>

class SampleApp : public Simply2D::Layer
{
public:
	void start() override;
	void destroy() override;
	void render() override;

private:
	std::unordered_map<std::string, Simply2D::AssetType> m_assets = {
		{"images/a.png", Simply2D::AssetType::IMAGE},
		{"images/b.png", Simply2D::AssetType::IMAGE},
		{"images/a.wav", Simply2D::AssetType::AUDIO},
		{"images/tileset1.bmp", Simply2D::AssetType::IMAGE},
		{"images/tileset2.png", Simply2D::AssetType::IMAGE},
		{"images/tileset3.png", Simply2D::AssetType::IMAGE}
	};

	Handle<Simply2D::Texture> m_texture1;
	Handle<Simply2D::Texture> m_texture2;
	Handle<Simply2D::Texture> m_texture3;

	Simply2D::TileSet* m_set{};
	Simply2D::TileSet* m_set1{};

	level1 m_level;
};