// Tile.hpp
#ifndef TILE_HPP
#define TILE_HPP

#include <SFML/Graphics.hpp>
#include <memory>
#include <string>

// Forward declaration to avoid circular dependency
class Building;

enum class TileType {
    Grass,
    Water,
    Road,
    Building
};

class Tile {
public:
    // Define TILE_WIDTH and TILE_HEIGHT as constants
    static const int TILE_WIDTH = 64; // Standard isometric width
    static const int TILE_HEIGHT = 32; // Standard isometric height

    Tile(int row, int col, TileType type = TileType::Grass);
    void setType(TileType type);
    TileType getType() const;
    void setBuilding(std::shared_ptr<Building> buildingPtr);
    std::shared_ptr<Building> getBuilding() const;
    void setPosition(float x, float y);
    sf::Vector2f getPosition() const;
    void draw(sf::RenderWindow& window) const;

private:
    // **Removed unused private fields 'row' and 'col'**
    // int row;
    // int col;
    TileType type;
    sf::Sprite sprite;
    std::shared_ptr<Building> building;

    void updateTexture();
};

#endif // TILE_HPP