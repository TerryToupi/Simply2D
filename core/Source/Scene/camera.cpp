#include "Scene/camera.h"
#include "Base/app.h"

#include "Source/Rendering/clipper.h"

namespace Simply2D
{
	static inline Clipper MakeClipper(Camera* camera)
	{
		return Clipper().SetView(
		[camera](void) -> const Rect&
			{ return static_cast<Rect>(camera->getViewPort()); }
		);
	}

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

	void Camera::Scroll(uint32_t dx, uint32_t dy, Span<TileLayer&> layers)
	{
		uint32_t cpyDx(dx);
		uint32_t cpyDy(dy);

		for (auto& layer : layers)
		{
			layer.filterScrollDistanceX(m_viewPort.x, m_viewPort.w, &cpyDx);
			layer.filterScrollDistanceY(m_viewPort.y, m_viewPort.h, &cpyDy);
		}
	
		m_viewPort.x += cpyDx;
		m_viewPort.y += cpyDy;
	}

	void Camera::BlitSprite(Rect& pos, Animator& animator)
	{
		Rect clippedBox;
		Rect dpyPos;
		
		if (MakeClipper(this).Clip(pos, { 0, 0, m_viewPort.w, m_viewPort.h }, &dpyPos, &clippedBox))
		{
			Rect clippedFrame
			{
				animator.GetActiveAnim()->frames[animator.GetCurrFrame()].x - clippedBox.x,
				animator.GetActiveAnim()->frames[animator.GetCurrFrame()].y - clippedBox.y,
				clippedBox.w,
				clippedBox.h
			};

			Application::GetRenderer()->draw({
				.target = m_texture, 
				.loadOp = LoadOp::LOAD, 
				.storeOp = StoreOp::STORE
				},
				{
					{
						.texture = animator.GetActiveAnim()->texture,
						.blend = Blend::BLEND,
						.src = clippedFrame,
						.dist = dpyPos
					}
				}
			);
		}
	}

	void Camera::ResetTexture()
	{
		Application::GetRenderer()->draw({
				.target = m_texture,
				.loadOp = Simply2D::LoadOp::CLEAR,
				.storeOp = Simply2D::StoreOp::CLEAR,
				.clearColor = 255,
			}, Span<DrawCall>());
	}
}