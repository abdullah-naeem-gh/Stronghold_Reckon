#ifndef MAP_HPP
#define MAP_HPP

#include "Tile.hpp"
#include "Building.hpp"
#include <vector>
#include <memory>
#include <stack>
#include "GameState.hpp"

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
    void saveState();
    void undo();
    void redo();

    std::vector<std::shared_ptr<Tile>> getNeighbors(std::shared_ptr<Tile> tile) const; // New method

private:
    int rows;
    int cols;
    std::vector<std::vector<std::shared_ptr<Tile>>> tiles;
    std::vector<std::shared_ptr<Building>> buildings;
    std::stack<GameState> undoStack;
    std::stack<GameState> redoStack;
    int nextBuildingId;

    void restoreGameState(const GameState& state);
    static const int START_X = 400;
    static const int START_Y = 50;
};

#endif // MAP_HPP