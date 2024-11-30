// Building.hpp
#ifndef BUILDING_HPP
#define BUILDING_HPP

#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>

// Forward declaration to avoid circular dependency (if necessary)
class Tile;

class Building {
public:
    Building(int id, float x, float y, const std::string& texturePath);
    int getId() const;
    void setPosition(float x, float y);
    sf::Vector2f getPosition() const;
    void draw(sf::RenderWindow& window) const;

private:
    int id;
    sf::Sprite sprite;
    // Additional properties like health, defense level, etc., can be added here
    static const int BUILDING_WIDTH = 64;  // Match tile width
    static const int BUILDING_HEIGHT = 64; // Adjust height as needed
};

#endif // BUILDING_HPP