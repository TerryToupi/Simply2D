#include <pch.h>

#include <iostream>
#include <fstream>
#include <iomanip>

#include <app.h>

#include <Source/renderingBackend.h>
#include <Source/assetDataBase.h>

#include <Source/mtJobSystem.h>

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
			handle = loadImage(fullPath).pack();
			break;
		case Simply2D::AssetType::AUDIO:
			handle = loadAudio(fullPath).pack();
			break;
		case Simply2D::AssetType::FONT:
			handle = laodFont(fullPath).pack();
			break;
		case Simply2D::AssetType::SERIALIZABLE:
			handle = loadSerializable(fullPath).pack();
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
			unloadImage(Handle<Image>(asset.handle));
			break;
		case Simply2D::AssetType::AUDIO:
			unloadAudio(Handle<Audio>(asset.handle));
			break;
		case Simply2D::AssetType::FONT:
			unloadFont(Handle<Font>(asset.handle));
			break;
		default:
			break;
		}
		m_loadedAssets.erase(path);
	}

	SDL_Surface* AssetDatabaseImpl::getImage(Handle<Image> image)
	{
		return *m_images.get(image);
	}

	MIX_Audio* AssetDatabaseImpl::getAudio(Handle<Audio> audio)
	{
		return *m_audio.get(audio);
	}

	TTF_Font* AssetDatabaseImpl::getFont(Handle<Font> font)
	{
		return *m_font.get(font);
	}

	json* AssetDatabaseImpl::getSerializable(Handle<Serializable> text)
	{
		return m_serializables.get(text);
	}

	Handle<Image> AssetDatabaseImpl::loadImage(std::string path)
	{
		SDL_Surface* surface = IMG_Load(path.c_str());
		if (!surface)
		{
			SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "FAILED: %s ", SDL_GetError());
			return Handle<Image>();
		}
		
		return m_images.insert(surface);
	}

	Handle<Audio> AssetDatabaseImpl::loadAudio(std::string path)
	{
		MIX_Audio* audio = MIX_LoadAudio(nullptr, path.c_str(), true);
		if (!audio)
		{
			SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "FAILED: %s ", SDL_GetError());
			return Handle<Audio>();
		}

		return m_audio.insert(audio);
	}

	Handle<Font> AssetDatabaseImpl::laodFont(std::string path)
	{
		TTF_Font* font = TTF_OpenFont(path.c_str(), 16);
		if (!font)
		{
			SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "FAILED: %s ", SDL_GetError());
			return Handle<Font>();
		} 

		return m_font.insert(font);
	}

	Handle<Serializable> AssetDatabaseImpl::loadSerializable(std::string path)
	{
		std::ifstream file(path);
		return m_serializables.insert(json::parse(file));
	}

	void AssetDatabaseImpl::unloadImage(Handle<Image> image)
	{
		SDL_Surface* surface = getImage(image);
		if (!surface)
			return;
	
		SDL_DestroySurface(surface);
		m_images.remove(image);
	}

	void AssetDatabaseImpl::unloadAudio(Handle<Audio> audio)
	{
		MIX_Audio* a = getAudio(audio);
		if (!a)
			return;

		MIX_DestroyAudio(a);
		m_audio.remove(audio);
	}

	void AssetDatabaseImpl::unloadFont(Handle<Font> font)
	{
		TTF_Font* f = getFont(font);
		if (!f)
			return;

		TTF_CloseFont(f);
		m_font.remove(font);
	}

	void AssetDatabaseImpl::unloadSerializable(Handle<Serializable> text)
	{ 
		json* j = getSerializable(text); 
		if (!j)
			return;

		j->clear();
		m_serializables.remove(text);
	}

	std::shared_ptr<AssetDatabase> AssetDatabase::Create(const AssetDatabaseSpecifications& specs)
	{
		return std::make_shared<AssetDatabaseImpl>(specs);
	}
}
