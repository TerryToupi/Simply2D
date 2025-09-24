#pragma once

#include <assets.h>

#include <cstdint>
#include <cmath>
#include <vector>

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
        using TileStorage = std::vector<Tile>;

    public:
        TileSet() = default;
        TileSet(uint16_t tileWidth, uint16_t tileHeight, Asset setImage);
        ~TileSet();

        Handle<Texture> texture();

        Tile getTile(uint16_t index);
        Tile getTile(uint16_t x, uint16_t y);

        uint16_t getTileWidth();
        uint16_t getTileHeight();

        uint16_t getTileSetWidth();
        uint16_t getTileSetHeight();

    private:
        Handle<Texture> m_tileSetTexture;

        uint16_t m_tileWidth = 0;
        uint16_t m_tileHeight = 0;
        uint16_t m_tileSetWidth = 0;
        uint16_t m_tileSetHeight = 0;

        TileStorage m_tiles;
    };
}