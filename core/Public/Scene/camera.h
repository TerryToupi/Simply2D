#pragma once

#include "Math/mathUtils.h"
#include "Types/Span.h"
#include "Types/Handle.h"
#include "Base/resources.h"
#include "Scene/tileLayers.h"
#include "Animations/animator.h"

namespace Simply2D
{

	class Camera
	{
	public:
		Camera(uint32_t x, uint32_t y, uint32_t w, uint32_t h);
		~Camera();

		void Scroll(uint32_t dx, uint32_t dy, uint32_t speedX, uint32_t speedY, TileLayer& layer);
		void BlitSprite(Rect& pos, Animator& animation);
		void BlitTileLayer();

		void ResetTexture();

		Rect getViewPort() const { return m_viewPort; }
		THandle<Texture> texture() const { return m_texture; }

	private:
		Rect m_viewPort;
		THandle<Texture> m_texture;
	};
}
