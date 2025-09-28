#pragma once

#include <functional>

#include <pool.h>
#include <renderer.h>

#include <Source/mtRingBuffer.h>

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

namespace Simply2D
{
	class RendererImpl final : public Renderer
	{
	public:
		RendererImpl(const RendererSpecifications& specs);
		~RendererImpl();

		virtual void draw(const RenderDescriptor& desc, Span<DrawCall> calls) override;

		virtual Handle<Texture> createTexture(const TextureDescriptor&& desc) override;
		virtual Handle<Texture> createTexture(Handle<Image> image) override;
		virtual void destroyTexture(Handle<Texture> texture) override;
		
		virtual void textureSize(Handle<Texture> texture, int& width, int& height) override;

		SDL_Texture* getTexture(Handle<Texture> texture);
		
	private:
		void beginFrame();
		void endFrame();

	private:
		SDL_Window* m_windowHandle{};
		SDL_Renderer* m_rendererHandle{};

		Pool<SDL_Texture*, Texture> m_textures{ 16u, "Texture Resources" };

		friend class Application;
	};
}