#ifndef TILE_HPP
#define TILE_HPP

#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>
#include <string>

class Building;

enum class TileType {
    Grass,
    Water,
    Road,
    Wall
    // Removed Building type
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

    // New methods
    void addEdge(std::shared_ptr<Tile> neighbor);
    const std::vector<std::shared_ptr<Tile>>& getNeighbors() const;
    std::shared_ptr<Tile> getNeighbor(int dx, int dy) const;

    void takeDamage(int damage);
    int getHealth() const;
    void setHealth(int health);
    bool isDestroyed() const;
    void setBlockStatus(bool status);

private:
    TileType type;
    int row;
    int col;
    bool blockStatus;
    sf::Sprite sprite;
    std::string texturePath; // Stores the texture path for non-grass tiles
    std::shared_ptr<Building> building;
    std::vector<std::shared_ptr<Tile>> neighbors; // List of edges to neighboring tiles
    void loadTexture(); // Helper method to load texture based on type and texturePath
    int health;

    // New member to store grass tile index for consistency
    int grassTileIndex;
};

#endif // TILE_HPP