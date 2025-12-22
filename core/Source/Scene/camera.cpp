#include "Scene/camera.h"
#include "Base/app.h"

namespace Simply2D
{
	Camera::Camera(uint32_t x, uint32_t y, uint32_t w, uint32_t h)
		: m_viewPort(x, y, w, h)
	{
		m_texture = Application::GetRenderer()->createTexture({
			.format = TextureFormat::RGBA8_UNORM,
			.access = TextureAccess::READ_WRITE,
			.width = (int)m_viewPort.w,
			.height = (int)m_viewPort.h,
		});
	}

	Camera::~Camera()
	{
		Application::GetRenderer()->destroyTexture(m_texture);
	}

	void Camera::Scroll(const uint32_t dx, const uint32_t dy, Span<TileLayer*> layers)
	{
		uint32_t cpyDx(dx);
		uint32_t cpyDy(dy);

		for (const auto& layer : layers)
		{
			layer->filterScrollDistance(m_viewPort.x, m_viewPort.w, &cpyDx, layer->getPixelWidth());
			layer->filterScrollDistance(m_viewPort.y, m_viewPort.h, &cpyDy, layer->getPixelHeight());
		}
	
		m_viewPort.x += cpyDx;
		m_viewPort.y += cpyDy;
	}
}