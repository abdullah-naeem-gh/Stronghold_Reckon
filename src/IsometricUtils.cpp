// IsometricUtils.cpp
#include "IsometricUtils.hpp"
#include "Tile.hpp" // To access TILE_WIDTH and TILE_HEIGHT

sf::Vector2f IsometricUtils::tileToScreen(int row, int col, float startX, float startY) {
    float x = (col - row) * (Tile::TILE_WIDTH / 2.0f) + startX;
    float y = (col + row) * (Tile::TILE_HEIGHT / 2.0f) + startY;
    return sf::Vector2f(x, y);
}

TileCoordinates IsometricUtils::screenToTile(float x, float y, float startX, float startY) {
    float fx = x - startX;
    float fy = y - startY;

    float col = (fx / (Tile::TILE_WIDTH / 2.0f) + fy / (Tile::TILE_HEIGHT / 2.0f)) / 2.0f;
    float row = (fy / (Tile::TILE_HEIGHT / 2.0f) - fx / (Tile::TILE_WIDTH / 2.0f)) / 2.0f;

    return TileCoordinates{ static_cast<int>(std::floor(row)), static_cast<int>(std::floor(col)) };
}