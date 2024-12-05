#ifndef TILE_HPP
#define TILE_HPP

#include <SFML/Graphics.hpp>
#include <memory>
#include <string>

class Building;

enum class TileType {
    Grass,
    Water,
    Road,
    Building
};

class Tile {
public:
    static const int TILE_WIDTH = 64;
    static const int TILE_HEIGHT = 32;

    Tile(int row, int col, TileType type = TileType::Grass);

    void setType(TileType type);
    TileType getType() const;
    void setBuilding(std::shared_ptr<Building> buildingPtr);
    std::shared_ptr<Building> getBuilding() const;
    void setPosition(float x, float y);
    sf::Vector2f getPosition() const;
    void draw(sf::RenderWindow& window) const;
    void updateTexture();
    
    bool isWalkable() const; // New method

    int getRow() const { return row; } // Getter for row
    int getCol() const { return col; } // Getter for col

private:
    TileType type;
    sf::Sprite sprite;
    std::shared_ptr<Building> building;
    int row, col;
};

#endif // TILE_HPP