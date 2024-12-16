#ifndef TANK_HPP
#define TANK_HPP

#include <SFML/Graphics.hpp>
#include <string>

class Tank {
public:
    Tank(float x, float y, const std::string& texturePath);
    void setPosition(float x, float y);
    sf::Vector2f getPosition() const;
    void draw(sf::RenderWindow& window) const;

    static const int TANK_WIDTH = 64;
    static const int TANK_HEIGHT = 64;

private:
    sf::Sprite sprite;
    std::string texturePath;
};

#endif // TANK_HPP