#pragma once

#include "Base/assets.h"
#include "Types/Pool.h"
#include "Types/HashMap.h"
#include "Types/String.h"
#include "types/Json.h"

// for now
#include <string>

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_mixer/SDL_mixer.h>
#include <SDL3_ttf/SDL_ttf.h>

namespace Simply2D
{
    class ResourcLocator
    {
    public:
        ResourcLocator(std::string& root)
            :   m_root(root) { }

		void Mount(const std::string& prefix, const std::string& directory);
		std::string Resolve(const std::string& vfp) const;

	private:
		std::string m_root;
		THashMap<std::string, std::string> m_mounts;
	};

	class AssetDatabaseImpl final : public AssetDatabase
	{
	public:
		AssetDatabaseImpl(const AssetDatabaseSpecifications& specs);
		~AssetDatabaseImpl();

		virtual THandle<Image> loadImage(const std::string&) override;
		virtual THandle<Font>  loadFont(const std::string&) override;
		virtual THandle<Audio> loadAudio(const std::string&) override;
		virtual THandle<Json>  loadSerializable(const std::string&) override;

		virtual void unloadImage(const std::string&) override;
		virtual void unloadFont(const std::string&) override;
		virtual void unloadAudio(const std::string&) override;
		virtual void unloadSerializable(const std::string&) override;

		SDL_Surface* getImage(THandle<Image> image);
		MIX_Audio*   getAudio(THandle<Audio> audio);
		TTF_Font*    getFont(THandle<Font> font);
		json*        getSerializable(THandle<Json> text);

		// Pixel access utilities
		uint32_t getPixelAt(THandle<Image> image, int x, int y);
		bool isPixelTransparent(THandle<Image> image, int x, int y);
		float getPixelBrightness(THandle<Image> image, int x, int y);  // Returns 0.0 (black) to 1.0 (white)
		void getImageSize(THandle<Image> image, int& width, int& height);

    private: 
        TPool<SDL_Surface*, Simply2D::Image> m_images;
        TPool<MIX_Audio*, Simply2D::Audio>   m_audio;
        TPool<TTF_Font*, Simply2D::Font>     m_font;
        TPool<json, Simply2D::Json>			 m_serializables;

        ResourcLocator m_locator;
		THashMap<std::string, uint32_t> m_loaded;
    };
}
