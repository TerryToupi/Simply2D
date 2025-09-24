#pragma once

#include <handle.h>

#define UINT_BIT_SHIFT(x) (1u << x)

namespace Simply2D
{
	// ------------ TEXTURE ----------------------------
	class Texture;
	#define SURFACE Handle<Simply2D::Texture>(UINT_MAX)

	enum class TextureFormat : unsigned int
	{
		RGBA32_FLOAT = UINT_BIT_SHIFT(0),  /**< 32-bit float RGBA */
		RGBA8_UNORM = UINT_BIT_SHIFT(1),  /**< 8-bit unsigned normalized RGBA */
		UNDEFINED = UINT_BIT_SHIFT(2)  /**< Undefined format */
	};

	enum class TextureAccess : unsigned int
	{
		READ_ONLY = UINT_BIT_SHIFT(0),
		READ_WRITE = UINT_BIT_SHIFT(1),
		UNDEFINED = UINT_BIT_SHIFT(2),
	};

	struct TextureDescriptor 
	{
		TextureFormat format = TextureFormat::UNDEFINED;
		TextureAccess access = TextureAccess::UNDEFINED;
		int width = 0;
		int height = 0;
	};

	// ------------ IMAGE ----------------------------
	class Image;
		
	// ------------ AUDIO ---------------------------- 
	class Audio;

	// ----------- FONT ------------------------------
	class Font;
}
