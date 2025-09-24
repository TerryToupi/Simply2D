#pragma once 

#include <memory>

#include <span.h>
#include <resources.h>

namespace Simply2D 
{
	class TileLayer
	{
	public:
		TileLayer(int id, Span<Texture> tileSets);
		~TileLayer();

	private:
		int m_id = -1;

	};
}