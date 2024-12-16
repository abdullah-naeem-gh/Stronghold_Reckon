#include "IsometricUtils.hpp"
#include "Tile.hpp"
#include <cmath>
#include <iostream>

// Convert tile indices to screen coordinates
sf::Vector2f IsometricUtils::tileToScreen(int row, int col, float startX, float startY) {
    float x = (col - row) * (Tile::TILE_WIDTH / 2.0f) + startX;
    float y = (col + row) * (Tile::TILE_HEIGHT / 2.0f) + startY;
    return sf::Vector2f(x, y);
}

// Improved method to convert screen coordinates to tile indices
TileCoordinates IsometricUtils::screenToTile(float x, float y, float startX, float startY) {
    // Adjust mouse position by subtracting the start offsets
    float fx = x - startX;
    float fy = y - startY;

    // Recalculate the row and column considering the isometric projection
    float col = (fx / (Tile::TILE_WIDTH / 2.0f) + fy / (Tile::TILE_HEIGHT / 2.0f)) / 2.0f;
    float row = (fy / (Tile::TILE_HEIGHT / 2.0f) - fx / (Tile::TILE_WIDTH / 2.0f)) / 2.0f;

    // Return rounded values for indices
    return TileCoordinates{ static_cast<int>(std::floor(row)), static_cast<int>(std::floor(col)) };
}