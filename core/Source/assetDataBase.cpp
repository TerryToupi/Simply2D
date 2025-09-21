#include <pch.h>

#include <app.h>

#include <Source/renderingBackend.h>
#include <Source/assetDataBase.h>

namespace core
{ 
	AssetDatabaseImpl::AssetDatabaseImpl(const AssetDatabaseSpecifications& specs)
		:	AssetDatabase(specs)
	{
		
	}

	AssetDatabaseImpl::~AssetDatabaseImpl()
	{
	}

	std::future<Handle<Texture>> AssetDatabaseImpl::loadTextureAsync(const std::string& path)
	{
		return std::async(std::launch::async, [this, path]() -> Handle<Texture> 
			{
				RendererImpl* renderer = (RendererImpl*)Application::GetInstance().GetRenderer().get();

				SDL_Surface* cpuTexture = IMG_Load(path.c_str());
				if (!cpuTexture)
				{
					SDL_LogError(SDL_LOG_CATEGORY_GPU, "[SDL gfx] failed to: %s", SDL_GetError());
					return Handle<Texture>();
				}
	
				Handle<Texture> handle = m_texture.generateHandle();

				renderer->enqueueTransfer([this, renderer, cpuTexture, handle]() -> void
					{
						SDL_Texture* texture = renderer->uploadCPUtextureToGPU(cpuTexture);

						SDL_DestroySurface(cpuTexture);

						if (texture == nullptr)
						{
							SDL_LogError(SDL_LOG_CATEGORY_GPU, "[SDL gfx] failed to: %s", SDL_GetError());
							return;
						}

						this->m_texture.insert(handle, texture);
					});
				
				return handle;
			});
	}

	std::future<Handle<Audio>> AssetDatabaseImpl::loadAudioAsync(const std::string& path)
	{
		return std::future<Handle<Audio>>();
	}

	std::future<Handle<Track>> AssetDatabaseImpl::loadTrackAsync(const std::string& path)
	{
		return std::future<Handle<Track>>();
	}

	std::future<Handle<Font>> AssetDatabaseImpl::loadFontAsync(const std::string& path)
	{
		return std::future<Handle<Font>>();
	}

	std::future<Handle<Texture>> AssetDatabaseImpl::createTexture(const TextureDescriptor&& desc)
	{
		return std::future<Handle<Texture>>();
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

	//std::future<Asset*> AssetDatabaseImpl::LoadAsync(const std::string& path, AssetType type)
	//{
 //       return std::async(std::launch::async, [this, path, type]() -> Asset* 
 //           {
	//			std::lock_guard<std::mutex> lock(m_assetsMutex);

	//			if (m_assets.find(path) != m_assets.end()) 
	//			{
	//				m_assets[path]->refCount++;
	//				return m_assets[path];
	//			}

	//			Asset* asset = new Asset();
	//			asset->type = type;
	//			asset->refCount = 1;

	//			switch (type) {
	//			case AssetType::TEXTURE:
	//				asset->resource = reinterpret_cast<void*>(IMG_Load(path.c_str()));
	//				break;
	//			case AssetType::AUDIO:
	//				// load audio via SDL_mixer
	//				break;
	//			case AssetType::FONT:
	//				// load font via SDL_ttf
	//				break;
	//			default:
	//				asset->resource = nullptr;
	//				break;
	//			}

	//			m_assets[path] = asset;
	//			return asset;
 //           });
	//}

	//void AssetDatabaseImpl::Unload(const std::string& path)
	//{
	//}
}
