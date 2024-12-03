// IsometricUtils.hpp
#ifndef ISOMETRICUTILS_HPP
#define ISOMETRICUTILS_HPP

#include <SFML/Graphics.hpp>
#include <cmath>

// Forward declaration to avoid circular dependency
class Tile;

struct TileCoordinates {
    int row;
    int col;
};

class IsometricUtils {
public:
    // Convert tile indices to screen coordinates
    static sf::Vector2f tileToScreen(int row, int col, float startX = 0.0f, float startY = 0.0f);
    
    // Convert screen coordinates to tile indices
    static TileCoordinates screenToTile(float x, float y, float startX = 0.0f, float startY = 0.0f);
};

#endif // ISOMETRICUTILS_HPP