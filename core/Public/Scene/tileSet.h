#pragma once

#include <cstdint>
#include <cmath>

#include "Base/assets.h"
#include "Types/Arrays.h"

namespace Simply2D
{
    struct Tile
    {
        uint16_t x = 0;
        uint16_t y = 0;
    };

    class TileSet
    {
    public:
        using TileStorage = TVector<Tile>;

    public:
        TileSet() = default;
        TileSet(uint16_t tileWidth, uint16_t tileHeight, THandle<Image> setImage);
        ~TileSet();

        THandle<Texture> texture();

        Tile getTile(uint16_t index);
        Tile getTile(uint16_t x, uint16_t y);

        uint16_t getTileWidth();
        uint16_t getTileHeight();

        uint16_t getTileSetWidth();
        uint16_t getTileSetHeight();

    private:
        THandle<Texture> m_tileSetTexture;

        uint16_t m_tileWidth = 0;
        uint16_t m_tileHeight = 0;
        uint16_t m_tileSetWidth = 0;
        uint16_t m_tileSetHeight = 0;

        TileStorage m_tiles;
    };
}