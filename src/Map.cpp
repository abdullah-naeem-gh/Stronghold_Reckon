// Map.cpp
#include "Map.hpp"
#include <iostream>

Map::Map(int rows, int cols) : rows(rows), cols(cols), nextBuildingId(1) {
    initializeTiles();
}

void Map::initializeTiles() {
    tiles.resize(rows, std::vector<std::shared_ptr<Tile>>(cols, nullptr));
    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < cols; ++col) {
            tiles[row][col] = std::make_shared<Tile>(row, col);
            // Calculate isometric position
            float x = (col - row) * (Tile::TILE_WIDTH / 2.0f) + START_X;
            float y = (col + row) * (Tile::TILE_HEIGHT / 2.0f) + START_Y;
            tiles[row][col]->setPosition(x, y);
        }
    }
}

std::shared_ptr<Tile> Map::getTile(int row, int col) const {
    if (row < 0 || row >= rows || col < 0 || col >= cols)
        return nullptr;
    return tiles[row][col];
}

std::shared_ptr<Building> Map::addBuilding(int row, int col, const std::string& buildingTexture) {
    auto tile = getTile(row, col);
    if (!tile) {
        std::cerr << "Invalid tile coordinates.\n";
        return nullptr;
    }
    if (tile->getBuilding()) {
        std::cerr << "Tile already has a building.\n";
        return nullptr;
    }
    float x = tile->getPosition().x;
    float y = tile->getPosition().y;
    auto building = std::make_shared<Building>(nextBuildingId++, x, y, buildingTexture);
    buildings.push_back(building);
    tile->setBuilding(building);
    return building;
}

void Map::draw(sf::RenderWindow& window) const {
    // Draw tiles in row-major order (top to bottom, left to right)
    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < cols; ++col) {
            tiles[row][col]->draw(window);
        }
    }
    // Buildings are drawn within Tile::draw()
}