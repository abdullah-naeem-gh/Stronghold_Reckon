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
    // Declare functions only
    static sf::Vector2f tileToScreen(int row, int col, float startX = 400.0f, float startY = 50.0f);
    static TileCoordinates screenToTile(float x, float y, float startX = 400.0f, float startY = 50.0f);
};

#endif // ISOMETRICUTILS_HPP