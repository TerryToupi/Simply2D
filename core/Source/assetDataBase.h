#pragma once

#include <assets.h>

namespace core
{
    class AssetDatabaseImpl final : public AssetDatabase
    {
    public:
        AssetDatabaseImpl(const AssetDatabaseSpecifications& specs);
        ~AssetDatabaseImpl();

        virtual std::future<Asset*> LoadAsync(const std::string& path, AssetType type) override;
        virtual void Unload(const std::string& path) override;

    private: 
        std::unordered_map<std::string, Asset*> m_assets;
        std::mutex m_assetsMutex;
    };
}
