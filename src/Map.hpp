#ifndef MAP_HPP
#define MAP_HPP

#include "Building.hpp" 
#include "Tile.hpp"
#include "GameState.hpp"
#include "GameStateManager.hpp"
#include <vector>
#include <memory>
#include <string>
#include <fstream>
#include <SFML/Graphics.hpp>
#include "IsometricUtils.hpp"
#include "TextureManager.hpp"

#include "BulletManager.hpp"


class Map {
public:
    // Map(int rows, int cols);
    Map(int rows, int cols, BulletManager& centralBulletManager);
    void initializeTiles();
    std::shared_ptr<Tile> getTile(int row, int col) const;
    bool addBuilding(int row, int col, const std::string& buildingTexture);
    bool addWall(int row, int col);
    void draw(sf::RenderWindow& window) const;
    int getRows() const;
    int getCols() const;
    void saveToFile(const std::string& filename);
    void loadFromFile(const std::string& filename);
    std::vector<std::vector<std::shared_ptr<Tile>>> getTiles();
    std::vector<std::shared_ptr<Tile>> getNeighbors(std::shared_ptr<Tile>& tile) const;
    void saveState();
    void undo();
    void redo();

    std::shared_ptr<Tile> findNearestWall(int startRow, int startCol) const;
    bool addTrap(int row, int col, const std::string& trapTexture);
    int nextBuildingId;
    int nextTowerId;


    bool addTower(int row, int col, const std::string& selectedBuildingTexture);
    std::vector<std::shared_ptr<Tower>> getTowers() const;


    // void loadTownHallAnimation();
    // void update(float deltaTime);
    // void draw(sf::RenderWindow& window) const;

private:
    int rows;
    int cols;
    std::vector<std::vector<std::shared_ptr<Tile>>> tiles;
    static std::vector<std::vector<TileType>> tileTypeMap; // Added static map

    std::vector<std::shared_ptr<Tower>> towers; // Vector of Towers

    GameStateManager stateManager;

    BulletManager& centralBulletManager;
    
    void restoreGameState(const GameState& state);



    // sf::Sprite townHallSprite;
    // std::vector<sf::IntRect> townHallFrames;
    // float frameTime = 0.1f;
    // float currentTime = 0.0f;
    // size_t currentFrame = 0;
    // bool playing = true;
};

#endif // MAP_HPP