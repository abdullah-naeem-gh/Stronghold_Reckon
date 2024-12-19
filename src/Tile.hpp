// Tile.hpp
#ifndef TILE_HPP
#define TILE_HPP

#include <SFML/Graphics.hpp>
#include <memory>
#include <string>

// Forward declaration
class Building;

enum class TileType {
    Grass,
    Water,
    Road,
    Building
};

class Tile {
public:
    static const int TILE_WIDTH = 64;  // Standard isometric width
    static const int TILE_HEIGHT = 32; // Standard isometric height

    Tile(int row, int col, TileType type = TileType::Grass);
    Tile(const Tile& other); // Copy constructor

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

    // Public members for direct access (optional)
    int row;
    int col;

private:
    TileType type;
    sf::Sprite sprite;
    std::shared_ptr<Building> building;
    bool blockStatus;
};

#endif // TILE_HPP