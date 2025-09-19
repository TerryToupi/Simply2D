#pragma once

#include <pool.h>
#include <Include/renderer.h>
#include <SDL3/SDL.h>

namespace core
{
	class RendererImpl final : public Renderer
	{
	public:
		RendererImpl(const RendererSpecifications& specs);
		~RendererImpl();

		virtual void beginFrame() override;
		virtual void endFrame() override;
		virtual void draw(const gfx::RenderDescriptor& desc, Span<gfx::DrawCall> calls) override;

		virtual Handle<Texture> createTexture(const TextureDescriptor& desc) override;
		virtual void destroyTexture(Handle<Texture> texture) override;
		SDL_Texture* getTexture(Handle<Texture> texture);

	private:
		SDL_Window* m_windowHandle{};
		SDL_Renderer* m_rendererHandle{};

		Pool<SDL_Texture*, Texture> m_Textures{ 16u, "Texture Resources" };
	};
}