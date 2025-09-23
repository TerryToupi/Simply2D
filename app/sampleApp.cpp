#include <sampleApp.h>

void SampleApp::start()
{
	for (auto& [path, type] : m_assets)
		Simply2D::assetDatabase().load(type, path);

	m_texture1 = Simply2D::gfx().createTexture({
		.access = core::TextureAccess::READ_WRITE,
		.width = 50 + 500 + 50 + 500 + 50,
		.height = 50 + 500 + 50 + 500 + 50,
		});

	auto image = Simply2D::assetDatabase().get("a.png");
	m_texture2 = Simply2D::gfx().createTexture(Handle<core::Image>(image.hanlde));

	auto image2 = Simply2D::assetDatabase().get("b.png");
	m_texture3 = Simply2D::gfx().createTexture(Handle<core::Image>(image2.hanlde));

	// simulating static level creation
	int texture2width = 0, texture2height = 0;
	Simply2D::gfx().textureSize(m_texture2, texture2width, texture2height);

	int texture3width = 0, texture3height = 0;
	Simply2D::gfx().textureSize(m_texture3, texture3width, texture3height);

	Simply2D::gfx().draw(
	{
		.target = m_texture1,
		.loadOp = core::LoadOp::CLEAR,
		.storeOp = core::StoreOp::STORE,
		.clearColor = {0, 0, 0, 255}
	},
	{
		{.texture = m_texture2, .src = {0, 0, texture2width, texture2height}, .dist ={50, 50, 500, 500}},
		{.texture = m_texture3, .src = {0, 0, texture3width, texture3height}, .dist ={600, 50, 500, 500}},
		{.texture = m_texture3, .src = {0, 0, texture3width, texture3height}, .dist ={50, 600, 500, 500}},
		{.texture = m_texture2, .src = {0, 0, texture2width, texture2height}, .dist ={600, 600, 500, 500}},
	});
}

void SampleApp::destroy()
{
	for (auto& [path, type] : m_assets)
		Simply2D::assetDatabase().unload(path);

	Simply2D::gfx().destroyTexture(m_texture1);
	Simply2D::gfx().destroyTexture(m_texture2);
	Simply2D::gfx().destroyTexture(m_texture3);
}

void SampleApp::render()
{
	int surfaceWidth = 0, surfaceHeight = 0;
	Simply2D::gfx().textureSize(SURFACE, surfaceWidth, surfaceHeight);

	int size = 50 + 500 + 50 + 500 + 50;

	Simply2D::gfx().draw(
	{
		.target = SURFACE,
		.loadOp = core::LoadOp::CLEAR,
		.storeOp = core::StoreOp::STORE,
		.clearColor = {50, 0, 0, 255}
	},
	{
		{.texture = m_texture1, .src = {0, 0, size, size}, .dist ={0, 0, surfaceWidth, surfaceHeight}},
	});
}
