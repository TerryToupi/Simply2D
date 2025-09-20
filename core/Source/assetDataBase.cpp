#include <pch.h>
#include <Source/assetDataBase.h>
#include <SDL3_image/SDL_image.h>

namespace core
{ 
	AssetDatabaseImpl::AssetDatabaseImpl(const AssetDatabaseSpecifications& specs)
		:	AssetDatabase(specs)
	{
		
	}

	AssetDatabaseImpl::~AssetDatabaseImpl()
	{
	}

	std::shared_ptr<AssetDatabase> AssetDatabase::Create(const AssetDatabaseSpecifications& specs)
	{
		return std::make_shared<AssetDatabaseImpl>(specs);
	}

	std::future<Asset*> AssetDatabaseImpl::LoadAsync(const std::string& path, AssetType type)
	{
        return std::async(std::launch::async, [this, path, type]() -> Asset* 
            {
				std::lock_guard<std::mutex> lock(m_assetsMutex);

				if (m_assets.find(path) != m_assets.end()) 
				{
					m_assets[path]->refCount++;
					return m_assets[path];
				}

				Asset* asset = new Asset();
				asset->type = type;
				asset->refCount = 1;

				switch (type) {
				case AssetType::TEXTURE:
					asset->resource = reinterpret_cast<void*>(IMG_Load(path.c_str()));
					break;
				case AssetType::AUDIO:
					// load audio via SDL_mixer
					break;
				case AssetType::FONT:
					// load font via SDL_ttf
					break;
				default:
					asset->resource = nullptr;
					break;
				}

				m_assets[path] = asset;
				return asset;
            });
	}

	void AssetDatabaseImpl::Unload(const std::string& path)
	{
	}
}
