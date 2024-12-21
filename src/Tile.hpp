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
    Building,
    Wall
};

class Tile {
public:
    static const int TILE_WIDTH = 64;
    static const int TILE_HEIGHT = 32;
    
    Tile(int row, int col, TileType type = TileType::Grass);
    Tile(const Tile& other);
    
    void setType(TileType type);
    TileType getType() const;
    
    void setBuilding(std::shared_ptr<Building> buildingPtr);
    std::shared_ptr<Building> getBuilding() const;
    
    void setPosition(float x, float y);
    sf::Vector2f getPosition() const;

    void draw(sf::RenderWindow& window) const;
    void updateTexture();
    
    int getRow() const;
    int getCol() const;
    
    bool isBlocked() const;
    
    // New method to add an edge to a neighboring tile
    void addEdge(std::shared_ptr<Tile> neighbor);
    // New method to get the neighbors
    const std::vector<std::shared_ptr<Tile>>& getNeighbors() const;
    
    bool isWall() const;

    // New methods
    std::shared_ptr<Tile> getNeighbor(int dx, int dy) const;
    void takeDamage(int damage);

private:
    TileType type;
    int row;
    int col;
    bool blockStatus;
    sf::Sprite sprite;
    std::shared_ptr<Building> building;
    std::vector<std::shared_ptr<Tile>> neighbors; // List of edges to neighboring tiles
};

#endif // TILE_HPP