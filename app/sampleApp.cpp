#include <sampleApp.h>

void SampleApp::start()
{
	m_texture = core::gfx().createTexture({
		.format = TextureFormat::RGBA8_UNORM,
		.access = TextureAccess::READ_WRITE,
		.width = 1920,
		.height = 1080
	});
}

void SampleApp::destroy()
{
	core::gfx().destroyTexture(m_texture);
}

void SampleApp::render()
{
	core::gfx().draw({
			.target = SURFACE,
			.loadOp = gfx::LoadOp::LOAD,
			.storeOp = gfx::StoreOp::CLEAR,
			.clearColor = {50, 0, 0, 255}
		},Span<gfx::DrawCall>());
}
