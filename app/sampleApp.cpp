#include <sampleApp.h>

void SampleApp::start()
{
	m_texture1 = core::assetDatabase().loadTextureAsync("/a.png");
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
			{.texture = m_texture1, .src = {0, 0, 640, 480}, .dist ={0, 0, 640, 480}}
		});
}
