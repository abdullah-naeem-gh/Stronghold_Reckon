// Map.hpp
#ifndef MAP_HPP
#define MAP_HPP

#include "Tile.hpp"
#include "Building.hpp"
#include "TextureManager.hpp"
#include <vector>
#include <memory>
#include <string>
#include "IsometricUtils.hpp"
#include <fstream>

class Map {
public:
    Map(int rows, int cols);
    void initializeTiles();
    std::shared_ptr<Tile> getTile(int row, int col) const;
    std::shared_ptr<Building> addBuilding(int row, int col, const std::string& buildingTexture);
    void draw(sf::RenderWindow& window) const;

    // **Added Getter Methods**
    int getRows() const;
    int getCols() const;

    void saveToFile(const std::string &filename);

    void loadFromFile(const std::string &filename);

private:
    int rows;
    int cols;
    std::vector<std::vector<std::shared_ptr<Tile>>> tiles;
    std::vector<std::shared_ptr<Building>> buildings;
    int nextBuildingId;
    const int START_X = 400; // Center the map horizontally
    const int START_Y = 50;  // Offset from the top
};

#endif // MAP_HPP