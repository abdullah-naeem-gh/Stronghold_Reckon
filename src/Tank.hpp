#ifndef TANK_HPP
#define TANK_HPP

#include <SFML/Graphics.hpp>
#include <string>
#include "Tile.hpp"

class Tank {
public:
    Tank(float x, float y, const std::string& texturePath, std::vector<std::shared_ptr<Tile>> path);
    void setPosition(float x, float y);
    sf::Vector2f getPosition() const;
    void draw(sf::RenderWindow& window) const;

    std::vector<std::shared_ptr<Tile>> path;

    void move(float deltaTime);

    static const int TANK_WIDTH = 64;
    static const int TANK_HEIGHT = 64;

private:
    sf::Sprite sprite;
    std::string texturePath;

    size_t currentPathIndex = 0;
    const float speed = 100.0f; // Speed of the tank

};

#endif // TANK_HPP