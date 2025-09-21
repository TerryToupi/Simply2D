#pragma once

#include <functional>

#include <pool.h>
#include <renderer.h>

#include <Source/mtRingBuffer.h>

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

namespace core
{
	class RendererImpl final : public Renderer
	{
	public:
		using Transfer = std::function<void()>;

		RendererImpl(const RendererSpecifications& specs);
		~RendererImpl();

		virtual void draw(const RenderDescriptor& desc, Span<DrawCall> calls) override;

		//virtual Handle<Texture> createTexture(const TextureDescriptor& desc) override;
		//virtual void destroyTexture(Handle<Texture> texture) override;
		//SDL_Texture* getTexture(Handle<Texture> texture);

		SDL_Texture* uploadCPUtextureToGPU(SDL_Surface* cpuTextue);
		void enqueueTransfer(const Transfer& func);

	private:
		void beginFrame();
		void endFrame();
		void flushTransferQueue();

	private:
		SDL_Window* m_windowHandle{};
		SDL_Renderer* m_rendererHandle{};

		Pool<SDL_Texture*, Texture> m_Textures{ 16u, "Texture Resources" };

		MTRingQueue<Transfer, 256> m_transferQueue;

		friend class Application;
	};
}