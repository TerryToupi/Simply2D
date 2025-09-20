#pragma once

#include <unordered_map>
#include <filesystem>
#include <string>
#include <future>
#include <mutex>

namespace assets
{
    enum class AssetType
    {
        Texture,
        Audio,
        Font,
    };

    struct Asset
    {
        void* resource;       // SDL_Texture*, SDL_Surface*, Mix_Chunk*, etc
        AssetType type;
        std::atomic<int> refCount;
    };
}

namespace core
{
    struct AssetDatabaseSpecifications
    {
        std::string assetsPath = "./";
        bool hotReload = false;
    }

    class AssetDatabase
    {
    public:
        static std::unique_ptr<AssetDatabase> Create(const AssetDatabaseSpecifications& specs);
        AssetDatabase(const AssetDatabaseSpecifications& specs = AssetDatabaseSpecifications())
            :   m_specifications(specs) {}
        virtual ~AssetDatabase() = default;

        std::future<assets::Asset*> LoadAsync(const std::string& path, AssetType type) = 0;
        void Unload(const std::string& path) = 0;
        
    protected:
        AssetDatabaseSpecifications m_specifications;
    };
}
