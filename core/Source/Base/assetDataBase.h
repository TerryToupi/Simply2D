#pragma once

#include "Base/assets.h"
#include "Types/Pool.h"

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_mixer/SDL_mixer.h>
#include <SDL3_ttf/SDL_ttf.h>

#include <nlohmann/json.hpp>
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

        SDL_Surface* getImage(THandle<Image> image);
        MIX_Audio*   getAudio(THandle<Audio> audio);
        TTF_Font*    getFont(THandle<Font> font);

        json*        getSerializable(THandle<Serializable> text);

    private: 
        THandle<Image>        loadImage(std::string path);
        THandle<Audio>        loadAudio(std::string path);
        THandle<Font>         laodFont(std::string path);
        THandle<Serializable> loadSerializable(std::string path);

        void unloadImage(THandle<Image> image);
        void unloadAudio(THandle<Audio> audio);
        void unloadFont(THandle<Font> font);
        void unloadSerializable(THandle<Serializable> text);

        TPool<SDL_Surface*, Simply2D::Image> m_images;
        TPool<MIX_Audio*, Simply2D::Audio>   m_audio;
        TPool<TTF_Font*, Simply2D::Font>     m_font;
        TPool<json, Simply2D::Serializable>  m_serializables;

        std::unordered_map<std::string, Asset> m_loadedAssets;
    };
}
