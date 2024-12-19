// IsometricUtils.hpp
#ifndef ISOMETRIC_UTILS_HPP
#define ISOMETRIC_UTILS_HPP

#include <SFML/Graphics.hpp>

// Structure to hold tile coordinates
struct TileCoordinates {
    int row;
    int col;
};

class IsometricUtils {
public:
    // Converts tile indices to screen coordinates based on centralized map start positions
    static sf::Vector2f tileToScreen(int row, int col);
    
    // Converts screen coordinates back to tile indices, ensuring they stay within map boundaries
    static TileCoordinates screenToTile(float x, float y, int maxRows = 64, int maxCols = 64);
    
    // Getters for map starting positions
    static float getMapStartX();
    static float getMapStartY();

private:
    // Centralized map starting positions
    static constexpr float MAP_START_X = 400.0f;
    static constexpr float MAP_START_Y = 50.0f;
};

#endif // ISOMETRIC_UTILS_HPP