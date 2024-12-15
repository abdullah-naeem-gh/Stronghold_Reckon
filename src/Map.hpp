#ifndef MAP_HPP
#define MAP_HPP

#include "Tile.hpp"
#include "Building.hpp"
#include <vector>
#include <memory>
#include <string>
#include <stack>
#include "IsometricUtils.hpp"
#include "GameState.hpp"
#include <fstream>

class Map {
public:
    Map(int rows, int cols);
    void initializeTiles();
    std::shared_ptr<Tile> getTile(int row, int col) const;
    std::shared_ptr<Building> addBuilding(int row, int col, const std::string& buildingTexture);
    void draw(sf::RenderWindow& window) const;
    int getRows() const;
    int getCols() const;
    void saveToFile(const std::string &filename);
    void loadFromFile(const std::string &filename);
    // Undo/Redo functionality
    void saveState();
    void undo();
    void redo();
private:
    int rows;
    int cols;
    std::vector<std::vector<std::shared_ptr<Tile>>> tiles;
    std::vector<std::shared_ptr<Building>> buildings;
    std::stack<GameState> undoStack;
    std::stack<GameState> redoStack;
    int nextBuildingId;
    const int START_X = 400;
    const int START_Y = 50;

    void restoreGameState(const GameState& state);
};

#endif // MAP_HPP