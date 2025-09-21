#include <pch.h>

#include <app.h>

#include <Source/renderingBackend.h>
#include <Source/assetDataBase.h>

#if defined(_WIN32) || defined(__ANDROID__)
	#define GFX_PLATFORM SDL_WINDOW_VULKAN
#elif defined(__APPLE__)
	#define GFX_PLATFORM SDL_WINDOW_METAL 
#else
	#define GFX_PLATFORM 0
#endif

namespace core
{
	std::shared_ptr<Renderer> Renderer::Create(const RendererSpecifications& specs)
	{
		return std::make_shared<RendererImpl>(specs);
	}

	RendererImpl::RendererImpl(const RendererSpecifications& specs)
		:	Renderer(specs)
	{
		SDL_WindowFlags flags = GFX_PLATFORM;

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

	SDL_Texture* RendererImpl::uploadCPUtextureToGPU(SDL_Surface* cpuTextue)
	{
		return SDL_CreateTextureFromSurface(m_rendererHandle, cpuTextue);
	}

	void RendererImpl::enqueueTransfer(const Transfer& func)
	{
		m_transferQueue.push_back(func);
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

	void RendererImpl::flushTransferQueue()
	{ 
		Transfer job;
		while (m_transferQueue.pop_front(job))
			job();
	}

	//Handle<Texture> RendererImpl::createTexture(const TextureDescriptor& desc)
	//{
	//	SDL_PixelFormat format = SDL_PIXELFORMAT_UNKNOWN;
	//	SDL_TextureAccess access = SDL_TEXTUREACCESS_STATIC;
	//	
	//	switch (desc.access)
	//	{
	//	case TextureAccess::READ_ONLY:
	//		access = SDL_TEXTUREACCESS_STATIC;
	//	case TextureAccess::READ_WRITE:
	//		access = SDL_TEXTUREACCESS_TARGET;
	//	}

	//	switch (desc.format)
	//	{
	//	case TextureFormat::RGBA32_FLOAT:
	//		format = SDL_PIXELFORMAT_RGBA32;
	//	case TextureFormat::RGBA8_UNORM:
	//		format = SDL_PIXELFORMAT_RGBA8888;
 //       case TextureFormat::UNDEFINED:
 //           format = SDL_PIXELFORMAT_RGBA8888;
	//	}

	//	return m_Textures.insert(SDL_CreateTexture(
	//		m_rendererHandle,
	//		format,
	//		access,
	//		desc.width,
	//		desc.height
	//	));
	//}

	//void RendererImpl::destroyTexture(Handle<Texture> texture)
	//{
	//	SDL_Texture* tex = getTexture(texture);
	//	if (tex != nullptr)
	//		SDL_DestroyTexture(tex);

	//	m_Textures.remove(texture);
	//}

	//SDL_Texture* RendererImpl::getTexture(Handle<Texture> texture)
	//{
	//	return *m_Textures.get(texture);
	//}

	void RendererImpl::draw(const RenderDescriptor& desc, Span<DrawCall> calls)
	{
		AssetDatabaseImpl* assets = (AssetDatabaseImpl*)(Application::GetInstance().GetAssetDatabase().get());

		if (desc.target == SURFACE)
			SDL_SetRenderTarget(m_rendererHandle, nullptr);
		else
			SDL_SetRenderTarget(m_rendererHandle, assets->getTexture(desc.target));

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
			SDL_RenderTexture(m_rendererHandle, assets->getTexture(call.texture), src.data(), dist.data());
		}
	}
}
