#pragma once

#include "Types/Span.h"
#include "Types/Handle.h"
#include "Base/resources.h"
#include "Scene/tileLayers.h"

namespace Simply2D
{
	struct ViewPort
	{
		uint32_t x = 0;
		uint32_t y = 0;
		uint32_t w = 0;
		uint32_t h = 0;

		ViewPort() = default;

		ViewPort(uint32_t _x, uint32_t _y, uint32_t _w, uint32_t _h)
			:	x(_x), y(_y), w(_w), h(_h) { }

		ViewPort(uint32_t _w, uint32_t _h)
			:	x(0), y(0), w(_w), h(_h) { }
	};

	class Camera
	{
	public:
		Camera(uint32_t x, uint32_t y, uint32_t w, uint32_t h);
		~Camera();

		void Scroll(const uint32_t dx, const uint32_t dy, Span<TileLayer*> layers);

		ViewPort getViewPort() const { return m_viewPort; }
		THandle<Texture> texture() const { return m_texture; }

	private:
		ViewPort		 m_viewPort;
		THandle<Texture> m_texture;
	};
}
