#pragma once

#include "Base/assets.h"
#include "Types/Pool.h"

#include <nlohmann/json.hpp>
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_mixer/SDL_mixer.h>
#include <SDL3_ttf/SDL_ttf.h>

using json = nlohmann::json;

namespace Simply2D
{
    class AssetDatabaseImpl final : public AssetDatabase
    {
    public:
        AssetDatabaseImpl(const AssetDatabaseSpecifications& specs);
        ~AssetDatabaseImpl();

        virtual void load(AssetType type, std::string path) override;
        virtual Asset get(std::string path) override;
        virtual void unload(std::string path) override;

        SDL_Surface* getImage(Handle<Image> image);
        MIX_Audio*   getAudio(Handle<Audio> audio);
        TTF_Font*    getFont(Handle<Font> font);

        json*        getSerializable(Handle<Serializable> text);

    private: 
        Handle<Image>        loadImage(std::string path);
        Handle<Audio>        loadAudio(std::string path);
        Handle<Font>         laodFont(std::string path);
        Handle<Serializable> loadSerializable(std::string path);

        void unloadImage(Handle<Image> image);
        void unloadAudio(Handle<Audio> audio);
        void unloadFont(Handle<Font> font);
        void unloadSerializable(Handle<Serializable> text);

        Pool<SDL_Surface*, Simply2D::Image> m_images        { 16u, "texture pool" };
        Pool<MIX_Audio*, Simply2D::Audio>   m_audio         { 16u, "audio pool" };
        Pool<TTF_Font*, Simply2D::Font>     m_font          { 16u, "fonts pool" };
        Pool<json, Simply2D::Serializable>  m_serializables { 64u, "serializables pool" };

        std::unordered_map<std::string, Asset> m_loadedAssets;
    };
}
