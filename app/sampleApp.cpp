#include <sampleApp.h>

void SampleApp::start()
{
	for (auto& [path, type] : m_assets)
		Simply2D::assetDatabase().load(type, path);

	m_texture1 = Simply2D::gfx().createTexture({
		.access = Simply2D::TextureAccess::READ_WRITE,
		.width = 50 + 500 + 50 + 500 + 50,
		.height = 50 + 500 + 50 + 500 + 50,
		});
;
	auto setImage1 = Simply2D::assetDatabase().get("tileset1.bmp");
	m_set = new Simply2D::TileSet(16, 16, setImage1);

	auto setImage2 = Simply2D::assetDatabase().get("tileset3.png");
	m_set1 = new Simply2D::TileSet(32, 32, setImage2);
}

void SampleApp::destroy()
{
	for (auto& [path, type] : m_assets)
		Simply2D::assetDatabase().unload(path);

	//Simply2D::gfx().destroyTexture(m_texture1);
	//Simply2D::gfx().destroyTexture(m_texture2);
	//Simply2D::gfx().destroyTexture(m_texture3);
}

void SampleApp::render()
{
	int surfaceWidth = 0, surfaceHeight = 0;
	Simply2D::gfx().textureSize(SURFACE, surfaceWidth, surfaceHeight);

	Simply2D::TileSet* activeSet = m_set1;

	Handle<Simply2D::Texture> tileSet = activeSet->texture();
	uint16_t tileWidth = activeSet->getTileWidth();
	uint16_t tileHeight = activeSet->getTileHeight();

	uint16_t tileScale = tileWidth * 2;

	std::vector<Simply2D::DrawCall> calls;
	for (uint16_t col = 0; col < activeSet->getTileSetHeight(); col++)
	{
		for (uint16_t row = 0; row < activeSet->getTileSetWidth(); row++)
		{
			Simply2D::Tile tile = activeSet->getTile(col * activeSet->getTileSetWidth() + row);
			calls.push_back({
				.texture = tileSet, 
				.src = {tile.x, tile.y, tileWidth, tileHeight}, 
				.dist = {row * tileScale, col * tileScale, tileScale, tileScale}
			});
		}
	}

	Simply2D::gfx().draw({
			.target = SURFACE,
			.loadOp = Simply2D::LoadOp::CLEAR,
			.storeOp = Simply2D::StoreOp::STORE,
			.clearColor = {50, 0, 0, 255}
		}, Span(calls.data(), calls.size()));
}
