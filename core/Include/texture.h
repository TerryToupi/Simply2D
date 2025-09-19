#pragma once

#include <handle.h>

#define UINT_BIT_SHIFT(x) (1u << x)

enum class TextureFormat : unsigned int
{
	RGBA32_FLOAT = UINT_BIT_SHIFT(0),  /**< 32-bit float RGBA */
	RGBA8_UNORM = UINT_BIT_SHIFT(1),  /**< 8-bit unsigned normalized RGBA */
	UNDEFINED = UINT_BIT_SHIFT(2)  /**< Undefined format */
};

enum class TextureAccess : unsigned int
{
	READ_ONLY = UINT_BIT_SHIFT(0),
	READ_WRITE = UINT_BIT_SHIFT(1)
};

class Texture;
#define SURFACE Handle<Texture>().unpack(UINT_MAX)

struct TextureDescriptor 
{
	TextureFormat format = TextureFormat::UNDEFINED;
	TextureAccess access = TextureAccess::READ_ONLY;
	int width = 0;
	int height = 0;
};
