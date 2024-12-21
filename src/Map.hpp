// Map.hpp
#ifndef MAP_HPP
#define MAP_HPP

#include "Building.hpp"     // Include Building.hpp first
#include "Tile.hpp"
#include "GameState.hpp"
#include <vector>
#include <memory>
#include <stack>
#include <string>
#include <fstream>
#include <SFML/Graphics.hpp> // Include SFML graphics
#include "IsometricUtils.hpp"

class Map {
public:
    Map(int rows, int cols);
    void initializeTiles();
    std::shared_ptr<Tile> getTile(int row, int col) const;
    // bool addBuilding(int row, int col, const std::string& buildingTexture);
    void draw(sf::RenderWindow& window) const;
    int getRows() const;
    int getCols() const;
    void saveToFile(const std::string &filename);
    void loadFromFile(const std::string &filename);
    std::vector<std::vector<std::shared_ptr<Tile>>> getTiles();
    std::vector<std::shared_ptr<Tile>> getNeighbors(std::shared_ptr<Tile>& tile) const;
    bool addBuilding(int row, int col, const std::string& buildingTexture, TileType type);


    // Undo/Redo functionality
    void saveState();
    void undo();
    void redo();

    bool addWall(int row, int col);

private:
    int rows;
    int cols;
    std::vector<std::vector<std::shared_ptr<Tile>>> tiles;
    std::stack<GameState> undoStack;
    std::stack<GameState> redoStack;
    int nextBuildingId;

    void restoreGameState(const GameState& state);
};

#endif // MAP_HPP