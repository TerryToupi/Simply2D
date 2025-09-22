#include <pch.h>

#include <assert.h>

#include <app.h>

#include <Source/renderingBackend.h>
#include <Source/assetDataBase.h>

namespace core
{
	std::shared_ptr<Renderer> Renderer::Create(const RendererSpecifications& specs)
	{
		return std::make_shared<RendererImpl>(specs);
	}

	RendererImpl::RendererImpl(const RendererSpecifications& specs)
		:	Renderer(specs)
	{
		SDL_WindowFlags flags = 0;

		if (m_specifications.borderless)
			flags |= (SDL_WINDOW_BORDERLESS | SDL_WINDOW_FULLSCREEN);
		else 
			flags |= SDL_WINDOW_RESIZABLE;

		flags |= SDL_WINDOW_INPUT_FOCUS;
		flags |= SDL_WINDOW_MOUSE_CAPTURE;
		
		if (!SDL_CreateWindowAndRenderer(m_specifications.name.c_str(),
										 m_specifications.width,
										 m_specifications.height,
										 flags,
										 &m_windowHandle,
										 &m_rendererHandle))
			SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "[SDL renderer] failed on: %s", SDL_GetError());
	
		if (m_specifications.vSync)
			SDL_SetRenderVSync(m_rendererHandle, true);
		else
			SDL_SetRenderVSync(m_rendererHandle, false);
	}

	RendererImpl::~RendererImpl()
	{
		SDL_DestroyRenderer(m_rendererHandle);
		SDL_DestroyWindow(m_windowHandle);
	}

	void RendererImpl::beginFrame()
	{
		SDL_SetRenderTarget(m_rendererHandle, nullptr);
		SDL_SetRenderDrawColor(m_rendererHandle, 10, 10, 10, 255);
		SDL_RenderClear(m_rendererHandle);
	}

	void RendererImpl::endFrame()
	{
		SDL_RenderPresent(m_rendererHandle);
	}

	void RendererImpl::draw(const RenderDescriptor& desc, Span<DrawCall> calls)
	{
		if (desc.target == SURFACE)
			SDL_SetRenderTarget(m_rendererHandle, nullptr);
		else
		{
			SDL_Texture* texture = getTexture(desc.target);
			assert(texture);

			SDL_SetRenderTarget(m_rendererHandle, texture);
		}

		if (desc.loadOp == LoadOp::CLEAR)
		{
			SDL_SetRenderDrawColor(m_rendererHandle, desc.clearColor[0], desc.clearColor[1], desc.clearColor[2], desc.clearColor[3]);
			SDL_RenderClear(m_rendererHandle);

			if (desc.storeOp == StoreOp::CLEAR)
				return;
		}

		if (desc.storeOp == StoreOp::CLEAR)
		{
			SDL_SetRenderDrawColor(m_rendererHandle, desc.clearColor[0], desc.clearColor[1], desc.clearColor[2], desc.clearColor[3]);
			SDL_RenderClear(m_rendererHandle);
			return;
		}
		
		for (const auto& call : calls)
		{
			std::array<SDL_FRect, 1> src = { 
				(float)call.src[0],(float)call.src[1], 
				(float)call.src[2], (float)call.src[3] 
			};
			std::array<SDL_FRect, 1> dist = { 
				(float)call.dist[0], (float)call.dist[1], 
				(float)call.dist[2], (float)call.dist[3] 
			};
		
			SDL_Texture* texture = getTexture(call.texture);
			assert(texture);

			if (!SDL_RenderTexture(m_rendererHandle, texture, src.data(), dist.data()))
				SDL_LogError(SDL_LOG_CATEGORY_RENDER, "Failed: %s", SDL_GetError());
		}
	}
	Handle<Texture> RendererImpl::createTexture(const TextureDescriptor&& desc)
	{
		SDL_PixelFormat format = SDL_PIXELFORMAT_RGBA8888;
		SDL_TextureAccess access = SDL_TEXTUREACCESS_STATIC;

		switch (desc.access)
		{
		case core::TextureAccess::READ_ONLY:
			access = SDL_TEXTUREACCESS_STATIC;
		case core::TextureAccess::READ_WRITE:
			access = SDL_TEXTUREACCESS_TARGET;
		case core::TextureAccess::UNDEFINED:
			break;
		default:
			break;
		}

		switch (desc.format)
		{
		case core::TextureFormat::RGBA32_FLOAT:
			format = SDL_PIXELFORMAT_RGBA32;
		case core::TextureFormat::RGBA8_UNORM:
			format = SDL_PIXELFORMAT_RGBA8888;
		case core::TextureFormat::UNDEFINED:
			break;
		default:
			break;
		}

		SDL_Texture* texture = SDL_CreateTexture(m_rendererHandle, format, access, desc.width, desc.height);
		if (texture == nullptr)
		{
			SDL_LogError(SDL_LOG_CATEGORY_RENDER, "FAILED: %s ", SDL_GetError());
			return Handle<Texture>();
		}
			
		return m_textures.insert(texture);
	}

	Handle<Texture> RendererImpl::createTexture(Handle<Image> image)
	{
		AssetDatabaseImpl* assets = static_cast<AssetDatabaseImpl*>(Application::GetInstance().GetAssetDatabase().get());

		SDL_Surface* surface = assets->getImage(image);
		assert(surface);

		SDL_Texture* texture = SDL_CreateTextureFromSurface(m_rendererHandle, surface);
		if (!texture)
		{
			SDL_LogError(SDL_LOG_CATEGORY_RENDER, "FAILED: %s ", SDL_GetError());
			return Handle<Texture>();
		}
		
		return m_textures.insert(texture);
	}

	SDL_Texture* RendererImpl::getTexture(Handle<Texture> texture)
	{
		return *m_textures.get(texture);
	}

	void RendererImpl::destroyTexture(Handle<Texture> texture)
	{
		SDL_Texture* tex = getTexture(texture);
		SDL_DestroyTexture(tex);
		m_textures.remove(texture);
	}

	void RendererImpl::textureSize(Handle<Texture> texture, int& width, int& height)
	{
		if (texture == SURFACE)
		{
			int m_width, m_height;
			SDL_GetWindowSize(m_windowHandle, &m_width, &m_height);
			width = m_width;
			height = m_height;
			return;
		}
		else
		{
			SDL_Texture* tex = getTexture(texture);
			assert(tex);

			float m_width, m_height;
			SDL_GetTextureSize(tex, &m_width, &m_height);
			width = (int)m_width;
			height = (int)m_height;
			return;
		}
	}
}
