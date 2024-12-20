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

// Function to convert tile coordinates to screen coordinates
   sf::Vector2f IsometricUtils::tileToScreen(int row, int col) {
       float x = (col - row) * (Tile::TILE_WIDTH / 2.f) + MAP_START_X;
       float y = (col + row) * (Tile::TILE_HEIGHT / 2.f) + MAP_START_Y;
       return sf::Vector2f(x, y);
   }

   // Function to convert screen coordinates to tile coordinates
   TileCoordinates IsometricUtils::screenToTile(float x, float y, int maxRows, int maxCols) {
       float fx = x - MAP_START_X;
       float fy = y - MAP_START_Y;
       float col = (fx / (Tile::TILE_WIDTH / 2.f) + fy / (Tile::TILE_HEIGHT / 2.f)) / 2.f;
       float row = (fy / (Tile::TILE_HEIGHT / 2.f) - fx / (Tile::TILE_WIDTH / 2.f)) / 2.f;

       // Ensure indices are within bounds
       int finalRow = std::max(0, std::min(static_cast<int>(std::floor(row)), maxRows - 1));
       int finalCol = std::max(0, std::min(static_cast<int>(std::floor(col)), maxCols - 1));
       return TileCoordinates{ finalRow + 1, finalCol };
   }