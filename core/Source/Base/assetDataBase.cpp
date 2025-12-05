#include <iostream>
#include <fstream>
#include <iomanip>

#include "Base/app.h"

#include "Source/Rendering/renderingBackend.h"
#include "Source/Base/assetDataBase.h"

#include "Memory/memory.h"

namespace Simply2D
{ 
	AssetDatabaseImpl::AssetDatabaseImpl(const AssetDatabaseSpecifications& specs)
		:	AssetDatabase(specs), m_locator(m_specifications.assetsPath)
	{
		m_locator.Mount("image://", "assets/images");
		m_locator.Mount("scene://", "assets/scenes");
		m_locator.Mount("font://", "assets/fonts");
		m_locator.Mount("audio://", "assets/audio");
		m_locator.Mount("save://", "assets/save");
	}

	AssetDatabaseImpl::~AssetDatabaseImpl()
	{
	}

	SDL_Surface* AssetDatabaseImpl::getImage(THandle<Image> image)
	{
		return *m_images.Get(image);
	}

	MIX_Audio* AssetDatabaseImpl::getAudio(THandle<Audio> audio)
	{
		return *m_audio.Get(audio);
	}

	TTF_Font* AssetDatabaseImpl::getFont(THandle<Font> font)
	{
		return *m_font.Get(font);
	}

	json* AssetDatabaseImpl::getSerializable(THandle<Json> text)
	{
		return m_serializables.Get(text);
	}

	THandle<Image> AssetDatabaseImpl::loadImage(const std::string& vfp)
	{
		static constexpr TArray<const char*, 3> supported = { ".png", ".bmp", ".jpg"};

		for (auto ext : supported)
		{
			std::string rp = m_locator.Resolve(vfp) + ext;
			if (m_loaded.find(rp) != m_loaded.end())
				return THandle<Image>(m_loaded.at(rp));
			else 
			{
				SDL_Surface* surface = IMG_Load(rp.c_str());
				if (surface)
				{
					THandle<Image> handle = m_images.Insert(std::move(surface));
					m_loaded[rp] = handle.Pack();
					return handle;
				}
			}
		}

		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "FAILED: %s ", SDL_GetError());
		return THandle<Image>();
	}

	THandle<Font> AssetDatabaseImpl::loadFont(const std::string& vfp)
	{
		static constexpr TArray<const char*, 2> supported = { ".ttf", ".otf" };

		for (auto ext : supported)
		{
			std::string rp = m_locator.Resolve(vfp) + ext;
			if (m_loaded.find(rp) != m_loaded.end())
				return THandle<Font>(m_loaded.at(rp));
			else
			{
				TTF_Font* font = TTF_OpenFont(rp.c_str(), 16);
				if (font)
				{
					THandle<Font> handle = m_font.Insert(std::move(font));
					m_loaded[rp] = handle.Pack();
					return handle;
				}
			}
		}

		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "FAILED: %s ", SDL_GetError());
		return THandle<Font>();
	}

	THandle<Audio> AssetDatabaseImpl::loadAudio(const std::string& vfp)
	{
		static constexpr TArray<const char*, 2> supported = { ".wav", ".mp3" };

		for (auto ext : supported)
		{
			std::string rp = m_locator.Resolve(vfp) + ext;
			if (m_loaded.find(rp) != m_loaded.end())
				return THandle<Audio>(m_loaded.at(rp));
			else
			{
				MIX_Audio* audio = MIX_LoadAudio(nullptr, rp.c_str(), true);
				if (audio)
				{
					THandle<Audio> handle = m_audio.Insert(std::move(audio));
					m_loaded[rp] = handle.Pack();
					return handle;
				}
			}
		}

		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "FAILED: %s ", SDL_GetError());
		return THandle<Audio>();
	}

	THandle<Json> AssetDatabaseImpl::loadSerializable(const std::string& vfp)
	{
		std::string rp = m_locator.Resolve(vfp) + ".json";
		if (m_loaded.find(rp) != m_loaded.end())
			return THandle<Json>(m_loaded.at(rp));

		std::ifstream file(rp);
		json j = json::parse(file);
		THandle<Json> handle = m_serializables.Insert(std::move(j));
		m_loaded[rp] = handle.Pack();
		return handle;
	}

	void AssetDatabaseImpl::unloadImage(const std::string& vfp)
	{
		static constexpr TArray<const char*, 3> supported = { ".png", ".bmp", ".jpg"};

		THandle<Image> handle;
		for (auto ext : supported)
		{
			std::string rp = m_locator.Resolve(vfp) + ext;
			if (m_loaded.find(rp) != m_loaded.end())
			{
				handle = THandle<Image>(m_loaded.at(rp));
				m_loaded.erase(rp);
				break;
			}
		}

		SDL_Surface* surface = getImage(handle);
		if (!surface)
			return;
	
		SDL_DestroySurface(surface);
		m_images.Remove(handle);
	}

	void AssetDatabaseImpl::unloadAudio(const std::string& vfp)
	{
		static constexpr TArray<const char*, 2> supported = { ".wav", ".mp3" };

		THandle<Audio> handle;
		for (auto ext : supported)
		{
			std::string rp = m_locator.Resolve(vfp) + ext;
			if (m_loaded.find(rp) != m_loaded.end())
			{
				handle = THandle<Audio>(m_loaded.at(rp));
				m_loaded.erase(rp);
				break;
			}
		}

		MIX_Audio* a = getAudio(handle);
		if (!a)
			return;

		MIX_DestroyAudio(a);
		m_audio.Remove(handle);
	}

	void AssetDatabaseImpl::unloadSerializable(const std::string& vfp)
	{
		std::string rp = m_locator.Resolve(vfp) + ".json";
		if (m_loaded.find(rp) != m_loaded.end())
		{
			THandle<Json> handle(m_loaded.at(rp));
			json* j = getSerializable(handle); 
			if (!j)
				return;

			j->clear();
			m_serializables.Remove(handle);
			m_loaded.erase(rp);
		}
	}

	void AssetDatabaseImpl::unloadFont(const std::string& vfp)
	{
		static constexpr TArray<const char*, 2> supported = { ".ttf", ".otf" };

		THandle<Font> handle;
		for (auto ext : supported)
		{
			std::string rp = m_locator.Resolve(vfp) + ext;
			if (m_loaded.find(rp) != m_loaded.end())
			{
				handle = THandle<Font>(m_loaded.at(rp));
				m_loaded.erase(rp);
				break;
			}
		}

		TTF_Font* f = getFont(handle);
		if (!f)
			return;

		TTF_CloseFont(f);
		m_font.Remove(handle);
	}

	AssetDatabase* AssetDatabase::Create(const AssetDatabaseSpecifications& specs)
	{
		return static_cast<AssetDatabase*>(MM::New<AssetDatabaseImpl>(specs));
	}

	void ResourcLocator::Mount(const std::string& prefix, const std::string& directory)
	{
		m_mounts[prefix] = directory;
	}

	std::string ResourcLocator::Resolve(const std::string& vfp) const
	{
		for (const auto& [prefix, dir] : m_mounts)
		{
			if (vfp.starts_with(prefix))
			{
				std::string remaining = vfp.substr(prefix.size());
				return m_root + "/" + dir + "/" + remaining;
			}
		}

		return "";
	}
}
