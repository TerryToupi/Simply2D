#pragma once

#include <unordered_map>
#include <filesystem>
#include <string>
#include <future>
#include <mutex>


namespace core
{
	enum class AssetType
	{
		TEXTURE,
		AUDIO,
		FONT,
	};

	struct Asset
	{
		void* resource = nullptr;       // SDL_Texture*, SDL_Surface*, Mix_Chunk*, etc
		AssetType type = AssetType::TEXTURE;
		std::atomic<int> refCount = 0;
	};

    struct AssetDatabaseSpecifications
    {
        std::string assetsPath = "./";
        bool hotReload = false;
    };

    class AssetDatabase
    {
    public:
        static std::shared_ptr<AssetDatabase> Create(const AssetDatabaseSpecifications& specs);
        AssetDatabase(const AssetDatabaseSpecifications& specs = AssetDatabaseSpecifications())
            :   m_specifications(specs) {}
        virtual ~AssetDatabase() = default;

        virtual std::future<Asset*> LoadAsync(const std::string& path, AssetType type) = 0;
        virtual void Unload(const std::string& path) = 0;
        
    protected:
        AssetDatabaseSpecifications m_specifications;
    };
}
