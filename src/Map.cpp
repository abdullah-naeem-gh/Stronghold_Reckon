// Map.cpp
#include "Map.hpp"
#include <iostream>
#include <cmath>

// **Ensure IsometricUtils.hpp is included**
#include "IsometricUtils.hpp"

Map::Map(int rows, int cols)
    : rows(rows), cols(cols), nextBuildingId(1) {
    initializeTiles();
}

void Map::initializeTiles() {
    tiles.resize(rows, std::vector<std::shared_ptr<Tile>>(cols, nullptr));
    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < cols; ++col) {
            tiles[row][col] = std::make_shared<Tile>(row, col);
            // **Pass integer row and col to tileToScreen**
            sf::Vector2f isoPos = IsometricUtils::tileToScreen(row, col, START_X, START_Y);
            tiles[row][col]->setPosition(isoPos.x, isoPos.y);
            
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
        std::cerr << "Invalid tile coordinates: (" << row << ", " << col << ").\n";
        return nullptr;
    }

    sf::Vector2f isoPos = IsometricUtils::tileToScreen(row, col, START_X, START_Y);

    // Position at the tile center bottom
    sf::Vector2f buildingPosition = sf::Vector2f(
        isoPos.x + Tile::TILE_WIDTH / 2.0f - Building::BUILDING_WIDTH / 2.0f,
        isoPos.y + Tile::TILE_HEIGHT - Building::BUILDING_HEIGHT / 2.0f
    );

    auto building = std::make_shared<Building>(nextBuildingId++, isoPos.x, isoPos.y, buildingTexture);
    building->setPosition(buildingPosition.x, buildingPosition.y);
    buildings.push_back(building);
    tile->setBuilding(building);

    return building;
}

void Map::draw(sf::RenderWindow& window) const {
    // Draw tiles in row-major order (top to bottom, left to right
    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < cols; ++col) {
            tiles[row][col]->draw(window);
        }
    }
    // Buildings are drawn within Tile::draw()
}

// **Implementation of Getter Methods**
int Map::getRows() const {
    return rows;
}

int Map::getCols() const {
    return cols;
}

void Map::saveToFile(const std::string& filename) {
    std::ofstream outFile(filename);
    if (!outFile) {
        std::cerr << "Error opening file for writing.\n";
        return;
    }

    // Save map dimensions
    outFile << rows << " " << cols << "\n";

    // Save each tile and its building if it exists
    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < cols; ++col) {
            auto tile = getTile(row, col);
            if (tile) {
                outFile << static_cast<int>(tile->getType()) << " ";
                auto building = tile->getBuilding();
                if (building) {
                    outFile << building->getId() << " " << building->getTexturePath() << " ";
                } else {
                    outFile << "-1 - ";
                }
            }
            outFile << "\n";
        }
    }
    outFile.close();
}

void Map::loadFromFile(const std::string& filename) {
    std::ifstream inFile(filename);
    if (!inFile) {
        std::cerr << "Error opening file for reading.\n";
        return;
    }

    inFile >> rows >> cols;
    tiles.clear();
    initializeTiles();

    int tileType, buildingId;
    std::string texturePath;
    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < cols; ++col) {
            inFile >> tileType >> buildingId >> texturePath;
            auto tile = getTile(row, col);
            tile->setType(static_cast<TileType>(tileType));

            if (buildingId != -1) {
                auto building = std::make_shared<Building>(buildingId, 0, 0, texturePath);
                building->setPosition(tile->getPosition().x, tile->getPosition().y);
                tile->setBuilding(building);
            }
        }
    }
    inFile.close();
}