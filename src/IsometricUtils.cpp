// IsometricUtils.cpp
#include "IsometricUtils.hpp"
#include "Tile.hpp"
#include <cmath>
#include <algorithm>

// Initialize static constexpr variables
constexpr float IsometricUtils::MAP_START_X;
constexpr float IsometricUtils::MAP_START_Y;

// Returns the centralized starting X position of the map
float IsometricUtils::getMapStartX() {
    return MAP_START_X;
}

// Returns the centralized starting Y position of the map
float IsometricUtils::getMapStartY() {
    return MAP_START_Y;
}

// Converts tile indices to screen coordinates based on centralized map start positions
sf::Vector2f IsometricUtils::tileToScreen(int row, int col) {
    float x = (col - row) * (Tile::TILE_WIDTH / 2.0f) + MAP_START_X;
    float y = (col + row) * (Tile::TILE_HEIGHT / 2.0f) + MAP_START_Y;
    return sf::Vector2f(x, y);
}

// Converts screen coordinates back to tile indices, ensuring they stay within map boundaries
TileCoordinates IsometricUtils::screenToTile(float x, float y, int maxRows, int maxCols) {
    float fx = x - MAP_START_X;
    float fy = y - MAP_START_Y;

    float col = (fx / (Tile::TILE_WIDTH / 2.0f) + fy / (Tile::TILE_HEIGHT / 2.0f)) / 2.0f;
    float row = (fy / (Tile::TILE_HEIGHT / 2.0f) - fx / (Tile::TILE_WIDTH / 2.0f)) / 2.0f;

    // Floor the values to get integer tile indices
    int finalRow = static_cast<int>(std::floor(row));
    int finalCol = static_cast<int>(std::floor(col));

    // Clamp the values to ensure they are within map boundaries
    finalRow = std::max(0, std::min(finalRow, maxRows - 1));
    finalCol = std::max(0, std::min(finalCol, maxCols - 1));

    return TileCoordinates{ finalRow, finalCol };
}