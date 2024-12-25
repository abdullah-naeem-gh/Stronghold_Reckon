#ifndef TILE_HPP
#define TILE_HPP

#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>
#include <string>
#include "Building.hpp"
#include "Tower.hpp"
#include "Trap.hpp"


enum class TileType {
    Grass,
    Water,
    Road,
    Wall,
    Trap,
    Tower
};

class Tile {
public:
    static const int TILE_WIDTH = 64;
    static const int TILE_HEIGHT = 32;

    Tile(int row, int col, TileType type = TileType::Grass);
    Tile(const Tile& other); // Copy constructor

    void setType(TileType type);
    TileType getType() const;

    void setBuilding(std::shared_ptr<Building> buildingPtr);
    std::shared_ptr<Building> getBuilding() const;

    void setTrap(std::shared_ptr<Trap> trapPtr);
    std::shared_ptr<Trap> getTrap() const;

    void setTower(std::shared_ptr<Tower> towerPtr);
    std::shared_ptr<Tower> getTower() const;

    void setGrassTileIndex(int index);
    int getGrassTileIndex() const; // This method provides the index

    void setPosition(float x, float y);
    sf::Vector2f getPosition() const;

    void setTexturePath(const std::string& path); // Setter for texture path
    std::string getTexturePath() const; // Getter for texture path

    void draw(sf::RenderWindow& window) const;
    void updateTexture();

    int getRow() const;
    int getCol() const;

    bool isBlocked() const;
    bool isWall() const;

    void addEdge(std::shared_ptr<Tile> neighbor);
    const std::vector<std::shared_ptr<Tile>>& getNeighbors() const;
    std::shared_ptr<Tile> getNeighbor(int dx, int dy) const;

    void takeDamage(float damage);
    int getHealth() const;
    void setHealth(int health);

    bool isDestroyed() const;
    void setBlockStatus(bool status);

    // // traps
    // void setTrap(const std::string& trapTexture);
    bool hasTrap() const;
    // void triggerTrap();

private:
    TileType type;
    int row;
    int col;
    bool blockStatus;
    sf::Sprite sprite;
    std::string texturePath;
    std::shared_ptr<Building> building;
    std::shared_ptr<Tower> tower; // Add Tower management
    std::vector<std::shared_ptr<Tile>> neighbors;
    void loadTexture();
    int health;
    int grassTileIndex;
    // // traps
    std::shared_ptr<Trap> trap;

    // std::string trapTexture;
    // bool trapActive;
};

#endif // TILE_HPP