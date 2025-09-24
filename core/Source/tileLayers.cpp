#include <pch.h>

#include <app.h>
#include <tileLayers.h>

namespace Simply2D
{
	TileLayer::TileLayer(const TileLayerSpecifications& specs, std::shared_ptr<TileSet> tileSet)
		:	m_specs(specs)
	{
		m_texture = Application::GetInstance().GetRenderer()->createTexture({
			.format = TextureFormat::RGBA8_UNORM,
			.access = TextureAccess::READ_WRITE,
			.width = m_specs.width * tileSet->getTileWidth(),
			.height = m_specs.height * tileSet->getTileHeight(),
		});
	}

	TileLayer::~TileLayer()
	{
		Application::GetInstance().GetRenderer()->destroyTexture(m_texture);
	}
}