#pragma once

#include <unordered_map>
#include <filesystem>
#include <string>

#include "Base/resources.h"

namespace Simply2D
{
    struct AssetDatabaseSpecifications
    {
        std::string assetsPath = "./";
    };

    class AssetDatabase
    {
    public:
        AssetDatabase(const AssetDatabaseSpecifications& specs = AssetDatabaseSpecifications())
            :   m_specifications(specs) {}
        virtual ~AssetDatabase() = default;

       template<typename T>
       THandle<T> load(const std::string& vfp);

	   virtual THandle<Image> loadImage(const std::string&) = 0;
	   virtual THandle<Font>  loadFont(const std::string&) = 0;
	   virtual THandle<Audio> loadAudio(const std::string&) = 0;
	   virtual THandle<Json>  loadSerializable(const std::string&) = 0;

	   template<typename T>
	   void unload(const std::string& vfp);

	   virtual void unloadImage(const std::string&) = 0;
	   virtual void unloadFont(const std::string&) = 0;
	   virtual void unloadAudio(const std::string&) = 0;
	   virtual void unloadSerializable(const std::string&) = 0;

    protected:
        AssetDatabaseSpecifications m_specifications;

    private:
		static AssetDatabase* Create(const AssetDatabaseSpecifications& specs);

        friend class Application;
    };


	// Loading
	template<>
	inline THandle<Image> AssetDatabase::load<Image>(const std::string& vfp) 
	{
		return loadImage(vfp);
	}

	template<>
	inline THandle<Font> AssetDatabase::load<Font>(const std::string& vfp) 
	{
		return loadFont(vfp);
	}

	template<>
	inline THandle<Audio> AssetDatabase::load<Audio>(const std::string& vfp) 
	{
		return loadAudio(vfp);
	}

	template<>
	inline THandle<Json> AssetDatabase::load<Json>(const std::string& vfp) 
	{
		return loadSerializable(vfp);
	}

	// Unloading
	template<>
	inline void AssetDatabase::unload<Image>(const std::string& vfp) 
	{
		unloadImage(vfp);
	}

	template<>
	inline void AssetDatabase::unload<Font>(const std::string& vfp) 
	{
		unloadFont(vfp);
	}

	template<>
	inline void AssetDatabase::unload<Audio>(const std::string& vfp) 
	{
		unloadAudio(vfp);
	}

	template<>
	inline void AssetDatabase::unload<Json>(const std::string& vfp) 
	{
		unloadSerializable(vfp);
	}
}
