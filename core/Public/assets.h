#pragma once

#include <unordered_map>
#include <filesystem>
#include <string>

#include <resources.h>

namespace Simply2D
{
    struct AssetDatabaseSpecifications
    {
        std::string assetsPath = "./";
    };

    enum class AssetType : uint16_t
    {
        IMAGE,
        AUDIO,
        FONT,
        SERIALIZABLE 
    };
    
    struct Asset
    {
        AssetType type;
        uint32_t handle;
    };
    
    class AssetDatabase
    {
    public:
        AssetDatabase(const AssetDatabaseSpecifications& specs = AssetDatabaseSpecifications())
            :   m_specifications(specs) {}
        virtual ~AssetDatabase() = default;

        virtual void load(AssetType type, std::string path) = 0;
        virtual Asset get(std::string path) = 0;
        virtual void unload(std::string path) = 0;

    protected:
        AssetDatabaseSpecifications m_specifications;

    private:
		static std::shared_ptr<AssetDatabase> Create(const AssetDatabaseSpecifications& specs);

        friend class Application;
    };
}
