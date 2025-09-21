#include <pch.h>

#include <app.h>

#include <Source/renderingBackend.h>
#include <Source/assetDataBase.h>

#include <Source/mtJobSystem.h>

namespace core
{ 
	AssetDatabaseImpl::AssetDatabaseImpl(const AssetDatabaseSpecifications& specs)
		:	AssetDatabase(specs)
	{
		
	}

	AssetDatabaseImpl::~AssetDatabaseImpl()
	{
	}

	Handle<Texture> AssetDatabaseImpl::loadTextureAsync(const std::string& path)
	{
		Handle<Texture> handle = m_texture.generateHandle();

		std::string fullPath = m_specifications.assetsPath + path;

		JobSystem::Execute([this, fullPath, handle]() -> void
			{
				RendererImpl* renderer = (RendererImpl*)Application::GetInstance().GetRenderer().get();

				SDL_Surface* cpuTexture = IMG_Load(fullPath.c_str());
				if (!cpuTexture)
				{
					SDL_LogError(SDL_LOG_CATEGORY_GPU, "[SDL gfx] failed to: %s", SDL_GetError());
					return;
				}

				renderer->enqueueTransfer([this, renderer, cpuTexture, handle]() -> void
					{
						SDL_Texture* texture = renderer->uploadCPUtextureToGPU(cpuTexture);

						SDL_DestroySurface(cpuTexture);

						if (texture == nullptr)
						{
							SDL_LogError(SDL_LOG_CATEGORY_GPU, "[SDL gfx] failed to: %s", SDL_GetError());
							this->m_texture.remove(handle);
							return;
						}

						this->m_texture.insert(handle, texture);
					});
			});

		return handle;
	}

	Handle<Audio> AssetDatabaseImpl::loadAudioAsync(const std::string& path)
	{
		return Handle<Audio>();
	}

	Handle<Track> AssetDatabaseImpl::loadTrackAsync(const std::string& path)
	{
		return Handle<Track>();
	}

	Handle<Font> AssetDatabaseImpl::loadFontAsync(const std::string& path)
	{
		return Handle<Font>();
	}

	Handle<Texture> AssetDatabaseImpl::createTexture(const TextureDescriptor&& desc)
	{
		return Handle<Texture>();
	}

	void AssetDatabaseImpl::remove(Handle<Texture> texture)
	{
	}

	void AssetDatabaseImpl::remove(Handle<Audio> audio)
	{
	}

	void AssetDatabaseImpl::remove(Handle<Track> track)
	{
	}

	void AssetDatabaseImpl::remove(Handle<Font> font)
	{
	}

	SDL_Texture* AssetDatabaseImpl::getTexture(Handle<Texture> texture)
	{
		return *m_texture.get(texture);
	}

	MIX_Audio* AssetDatabaseImpl::getAudio(Handle<Audio> audio)
	{
		return nullptr;
	}

	MIX_Track* AssetDatabaseImpl::getTrack(Handle<Track> track)
	{
		return nullptr;
	}

	TTF_Font* AssetDatabaseImpl::getFont(Handle<Font> font)
	{
		return nullptr;
	}

	std::shared_ptr<AssetDatabase> AssetDatabase::Create(const AssetDatabaseSpecifications& specs)
	{
		return std::make_shared<AssetDatabaseImpl>(specs);
	}
}
