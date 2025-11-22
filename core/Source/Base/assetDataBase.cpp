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
		:	AssetDatabase(specs)
	{
	}

	AssetDatabaseImpl::~AssetDatabaseImpl()
	{
	}

	void AssetDatabaseImpl::load(AssetType type, std::string path)
	{
		if (m_loadedAssets.find(path) != m_loadedAssets.end())
			return;

		uint32_t handle = 0;
		std::string fullPath = m_specifications.assetsPath + path;

		switch (type)
		{
		case Simply2D::AssetType::IMAGE:
			handle = loadImage(fullPath).Pack();
			break;
		case Simply2D::AssetType::AUDIO:
			handle = loadAudio(fullPath).Pack();
			break;
		case Simply2D::AssetType::FONT:
			handle = laodFont(fullPath).Pack();
			break;
		case Simply2D::AssetType::SERIALIZABLE:
			handle = loadSerializable(fullPath).Pack();
		default:
			break;
		}
		
		m_loadedAssets.insert({ path, {type, handle} });
		return;
	}

	Asset AssetDatabaseImpl::get(std::string path)
	{
		return m_loadedAssets.at(path);
	}

	void AssetDatabaseImpl::unload(std::string path)
	{ 
		if (m_loadedAssets.find(path) != m_loadedAssets.end())
			return;
		
		Asset asset = m_loadedAssets.at(path);
		switch (asset.type)
		{
		case Simply2D::AssetType::IMAGE:
			unloadImage(THandle<Image>(asset.handle));
			break;
		case Simply2D::AssetType::AUDIO:
			unloadAudio(THandle<Audio>(asset.handle));
			break;
		case Simply2D::AssetType::FONT:
			unloadFont(THandle<Font>(asset.handle));
			break;
		default:
			break;
		}
		m_loadedAssets.erase(path);
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

	json* AssetDatabaseImpl::getSerializable(THandle<Serializable> text)
	{
		return m_serializables.Get(text);
	}

	THandle<Image> AssetDatabaseImpl::loadImage(std::string path)
	{
		SDL_Surface* surface = IMG_Load(path.c_str());
		if (!surface)
		{
			SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "FAILED: %s ", SDL_GetError());
			return THandle<Image>();
		}
		
		return m_images.Insert(std::move(surface));
	}

	THandle<Audio> AssetDatabaseImpl::loadAudio(std::string path)
	{
		MIX_Audio* audio = MIX_LoadAudio(nullptr, path.c_str(), true);
		if (!audio)
		{
			SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "FAILED: %s ", SDL_GetError());
			return THandle<Audio>();
		}

		return m_audio.Insert(std::move(audio));
	}

	THandle<Font> AssetDatabaseImpl::laodFont(std::string path)
	{
		TTF_Font* font = TTF_OpenFont(path.c_str(), 16);
		if (!font)
		{
			SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "FAILED: %s ", SDL_GetError());
			return THandle<Font>();
		} 

		return m_font.Insert(std::move(font));
	}

	THandle<Serializable> AssetDatabaseImpl::loadSerializable(std::string path)
	{
		std::ifstream file(path);
		json j = json::parse(file);
		return m_serializables.Insert(std::move(j));
	}

	void AssetDatabaseImpl::unloadImage(THandle<Image> image)
	{
		SDL_Surface* surface = getImage(image);
		if (!surface)
			return;
	
		SDL_DestroySurface(surface);
		m_images.Remove(image);
	}

	void AssetDatabaseImpl::unloadAudio(THandle<Audio> audio)
	{
		MIX_Audio* a = getAudio(audio);
		if (!a)
			return;

		MIX_DestroyAudio(a);
		m_audio.Remove(audio);
	}

	void AssetDatabaseImpl::unloadFont(THandle<Font> font)
	{
		TTF_Font* f = getFont(font);
		if (!f)
			return;

		TTF_CloseFont(f);
		m_font.Remove(font);
	}

	void AssetDatabaseImpl::unloadSerializable(THandle<Serializable> text)
	{ 
		json* j = getSerializable(text); 
		if (!j)
			return;

		j->clear();
		m_serializables.Remove(text);
	}

	AssetDatabase* AssetDatabase::Create(const AssetDatabaseSpecifications& specs)
	{
		return static_cast<AssetDatabase*>(New<AssetDatabaseImpl>(specs));
	}
}
