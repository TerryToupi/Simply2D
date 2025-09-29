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
	void update(float ts) override;
	void destroy() override;
	void render() override;

private:
	std::unordered_map<std::string, Simply2D::AssetType> m_assets = 
	{
		{"scenes/level1/Level1.json", Simply2D::AssetType::SCENE},
	};
};