#include <sampleApp.h>

void SampleApp::start()
{
	m_texture1 = core::assetDatabase().loadTextureAsync("a.png").get();
	m_texture2 = core::assetDatabase().loadTextureAsync("a.png").get();

	//m_texture = core::gfx().createTexture({
	//	.format = core::TextureFormat::RGBA8_UNORM,
	//	.access = core::TextureAccess::READ_WRITE,
	//	.width = 1920,
	//	.height = 1080
	//});
}

void SampleApp::destroy()
{
	//core::gfx().destroyTexture(m_texture);
}

void SampleApp::render()
{
	core::gfx().draw({
			.target = SURFACE,
			.loadOp = core::LoadOp::CLEAR,
			.storeOp = core::StoreOp::STORE,
			.clearColor = {50, 0, 0, 255}
		},
		{
			{.texture = m_texture1, .src = {50, 50, 50, 50}, .dist ={0, 0, 640, 480}}
		});
}
