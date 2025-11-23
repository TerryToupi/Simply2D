#include "Base/app.h"
#include "Base/assets.h"

#include "Source/Rendering/renderingBackend.h"
#include "Source/Base/assetDataBase.h"

#include "Memory/memory.h"

namespace Simply2D
{
	Renderer* Renderer::Create(const RendererSpecifications& specs)
	{
		return static_cast<Renderer*>(MM::New<RendererImpl>(specs));
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
        SDL_Texture* renderTarget = desc.target == SURFACE ? nullptr : getTexture(desc.target);
        
        SDL_SetRenderTarget(m_rendererHandle, renderTarget);
        SDL_SetRenderDrawColor(m_rendererHandle,
                               (uint8_t)desc.clearColor[0],
                               (uint8_t)desc.clearColor[1],
                               (uint8_t)desc.clearColor[2],
                               (uint8_t)desc.clearColor[3]);
        
        if (desc.loadOp == LoadOp::CLEAR && desc.storeOp == StoreOp::CLEAR)
        {
            if (!SDL_RenderClear(m_rendererHandle))
                SDL_LogError(SDL_LOG_CATEGORY_RENDER, "Failed LOAD::CLEAR STORE::CLEAR : %s", SDL_GetError());
            
            return;
        }
        
        if (desc.loadOp == LoadOp::CLEAR)
        {
            if (!SDL_RenderClear(m_rendererHandle))
                SDL_LogError(SDL_LOG_CATEGORY_RENDER, "Failed LOAD::CLEAR : %s", SDL_GetError());
        }
        
        if (desc.storeOp == StoreOp::CLEAR)
        {
            if (!SDL_RenderClear(m_rendererHandle))
                SDL_LogError(SDL_LOG_CATEGORY_RENDER, "Failed STORE::STRE : %s", SDL_GetError());
            
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

			SDL_BlendMode mode = SDL_BLENDMODE_BLEND;
			switch (call.blend)
			{
				case Blend::BLEND:  mode = SDL_BLENDMODE_BLEND; break;
				case Blend::ADD:	mode = SDL_BLENDMODE_ADD;	break;
				case Blend::MOD:	mode = SDL_BLENDMODE_MOD;	break;
				case Blend::NONE:	mode = SDL_BLENDMODE_NONE;	break;
			}
            
			if (!SDL_SetTextureBlendMode(texture, mode))
				SDL_LogError(SDL_LOG_CATEGORY_RENDER, "Failed: %s", SDL_GetError());

			if (!SDL_SetTextureAlphaMod(texture, call.alpha))
				SDL_LogError(SDL_LOG_CATEGORY_RENDER, "Failed: %s", SDL_GetError());
            
			if (!SDL_RenderTexture(m_rendererHandle, texture, src.data(), dist.data()))
				SDL_LogError(SDL_LOG_CATEGORY_RENDER, "Failed: %s", SDL_GetError());
		}
	}

	THandle<Texture> RendererImpl::createTexture(const TextureDescriptor&& desc)
	{
		SDL_PixelFormat format = SDL_PIXELFORMAT_RGBA8888;
		SDL_TextureAccess access = SDL_TEXTUREACCESS_STATIC;

		switch (desc.access)
		{
		case Simply2D::TextureAccess::READ_ONLY:
			access = SDL_TEXTUREACCESS_STATIC;
		case Simply2D::TextureAccess::READ_WRITE:
			access = SDL_TEXTUREACCESS_TARGET;
		case Simply2D::TextureAccess::UNDEFINED:
			break;
		default:
			break;
		}

		switch (desc.format)
		{
		case Simply2D::TextureFormat::RGBA32_FLOAT:
			format = SDL_PIXELFORMAT_RGBA32;
		case Simply2D::TextureFormat::RGBA8_UNORM:
			format = SDL_PIXELFORMAT_RGBA8888;
		case Simply2D::TextureFormat::UNDEFINED:
			break;
		default:
			break;
		}

		SDL_Texture* texture = SDL_CreateTexture(m_rendererHandle, format, access, desc.width, desc.height);
		if (texture == nullptr)
		{
			SDL_LogError(SDL_LOG_CATEGORY_RENDER, "FAILED: %s ", SDL_GetError());
			return THandle<Texture>();
		}
			
		return m_textures.Insert(std::move(texture));
	}

	THandle<Texture> RendererImpl::createTexture(THandle<Image> image)
	{
		AssetDatabaseImpl* assets = static_cast<AssetDatabaseImpl*>(Application::GetInstance()->GetAssetDatabase());

		SDL_Surface* surface = assets->getImage(image);
		assert(surface);

		SDL_Texture* texture = SDL_CreateTextureFromSurface(m_rendererHandle, surface);
		if (!texture)
		{
			SDL_LogError(SDL_LOG_CATEGORY_RENDER, "FAILED: %s ", SDL_GetError());
			return THandle<Texture>();
		}
		
		return m_textures.Insert(std::move(texture));
	}

	SDL_Texture* RendererImpl::getTexture(THandle<Texture> texture)
	{
		return *m_textures.Get(texture);
	}

	void RendererImpl::destroyTexture(THandle<Texture> texture)
	{
		SDL_Texture* tex = getTexture(texture);
		SDL_DestroyTexture(tex);
		m_textures.Remove(texture);
	}

	void RendererImpl::textureSize(THandle<Texture> texture, int& width, int& height)
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
