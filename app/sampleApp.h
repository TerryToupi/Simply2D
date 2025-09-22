#pragma once

#include <app.h> 

#include <string>
#include <unordered_map>

class SampleApp : public core::Layer
{
public:
	void start() override;
	void destroy() override;
	void render() override;

private:
	std::unordered_map<std::string, core::AssetType> m_assets = { 
		{"a.png", core::AssetType::IMAGE},
		{"b.png", core::AssetType::IMAGE},
		{"a.wav", core::AssetType::AUDIO},
	};

	Handle<core::Texture> m_texture1;
	Handle<core::Texture> m_texture2; 
	Handle<core::Texture> m_texture3;
};