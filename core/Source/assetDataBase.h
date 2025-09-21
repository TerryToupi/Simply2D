#pragma once

#include <assets.h>
#include <pool.h>
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_mixer/SDL_mixer.h>
#include <SDL3_ttf/SDL_ttf.h>

namespace core
{
    class AssetDatabaseImpl final : public AssetDatabase
    {
    public:
        AssetDatabaseImpl(const AssetDatabaseSpecifications& specs);
        ~AssetDatabaseImpl();

        virtual Handle<Texture> loadTextureAsync(const std::string& path) override;
        virtual Handle<Audio>   loadAudioAsync(const std::string& path) override;
        virtual Handle<Track>   loadTrackAsync(const std::string& path) override;
        virtual Handle<Font>    loadFontAsync(const std::string& path) override;

        virtual Handle<Texture> createTexture(const TextureDescriptor&& desc) override;

        virtual void remove(Handle<Texture> texture) override;
        virtual void remove(Handle<Audio> audio) override;
        virtual void remove(Handle<Track> track) override;
        virtual void remove(Handle<Font> font) override;

        SDL_Texture*    getTexture(Handle<Texture> texture);
        MIX_Audio*      getAudio(Handle<Audio> audio);
        MIX_Track*      getTrack(Handle<Track> track);
        TTF_Font*       getFont(Handle<Font> font);

    private: 
        Pool<SDL_Texture*, core::Texture>   m_texture{ 16u, "texture pool" };
        Pool<MIX_Audio*, core::Audio>       m_audio{ 8u, "audio pool" };
        Pool<MIX_Track*, core::Track>       m_track{ 4u, "tracks pool" };
        Pool<TTF_Font*, core::Font>         m_font{ 2u, "fonts pool" };
    };
}
