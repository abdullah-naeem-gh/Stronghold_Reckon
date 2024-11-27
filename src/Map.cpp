#include "Map.hpp"
#include <iostream>

const int TILE_WIDTH = 30; // Adjust based on your tile size
const int TILE_HEIGHT = 15; // Adjust based on your tile size

Map::Map() {
    if (!mapTexture.loadFromFile("../assets/green_surface.png")) {
        std::cerr << "Error loading texture" << std::endl;
    }
    mapSprite.setTexture(mapTexture);
    mapSprite.setScale(
        static_cast<float>(TILE_WIDTH) / mapTexture.getSize().x,
        static_cast<float>(TILE_HEIGHT) / mapTexture.getSize().y
    );
}

void Map::draw(sf::RenderWindow &window) {
    int numRows = 25; // Number of rows in the map
    int numCols = 25; // Number of columns in the map

    // Calculate the total width and height of the map
    int mapWidth = (numCols + numRows) * TILE_WIDTH / 2;
    int mapHeight = (numCols + numRows) * TILE_HEIGHT / 2;

    // Calculate the starting position to center the map
    int startX = window.getSize().x / 2 - mapWidth / 2 + 350;
    int startY = window.getSize().y / 2 - mapHeight / 2;

    // Iterate through each tile and draw it
    for (int row = 0; row < numRows; ++row) {
        for (int col = 0; col < numCols; ++col) {
            // Calculate the position for each tile
            int x = (col - row) * TILE_WIDTH / 2 + startX;
            int y = (col + row) * TILE_HEIGHT / 2 + startY;

            // Set the position of the sprite and draw it
            mapSprite.setPosition(static_cast<float>(x), static_cast<float>(y));
            window.draw(mapSprite);
        }
    }
}