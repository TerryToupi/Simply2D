#pragma once

#include <functional>

#include "Types/Pool.h"
#include "Rendering/renderer.h"

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

namespace Simply2D
{
	class GridLayer;

	class RendererImpl final : public Renderer
	{
	public:
		RendererImpl(const RendererSpecifications& specs);
		~RendererImpl();

		virtual void draw(const RenderDescriptor& desc, Span<DrawCall> calls) override;

		virtual THandle<Texture> createTexture(const TextureDescriptor&& desc) override;
		virtual THandle<Texture> createTexture(THandle<Image> image) override;
		virtual void destroyTexture(THandle<Texture> texture) override;
		
		virtual void textureSize(THandle<Texture> texture, int& width, int& height) override;

		SDL_Texture* getTexture(THandle<Texture> texture);
		
	private:
		void beginFrame();
		void endFrame();

	private:
		SDL_Window* m_windowHandle{};
		SDL_Renderer* m_rendererHandle{};

		TPool<SDL_Texture*, Texture> m_textures;

		friend class Application;
		friend class GridLayer;
	};
}