#pragma once

#include <unordered_map>
#include <filesystem>
#include <string>
#include <future>
#include <mutex>

#include <resources.h>

namespace core
{
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

        virtual Handle<Texture> loadTextureAsync(const std::string& path) = 0;
        virtual Handle<Audio>   loadAudioAsync(const std::string& path) = 0;
        virtual Handle<Track>   loadTrackAsync(const std::string& path) = 0;
        virtual Handle<Font>    loadFontAsync(const std::string& path) = 0;
        
		virtual Handle<Texture> createTexture(const TextureDescriptor&& desc) = 0;

        virtual void remove(Handle<Texture> texture) = 0;
        virtual void remove(Handle<Audio> audio) = 0;
        virtual void remove(Handle<Track> track) = 0;
        virtual void remove(Handle<Font> font) = 0;

    protected:
        AssetDatabaseSpecifications m_specifications;
    };
}
