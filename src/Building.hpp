#ifndef BUILDING_HPP
#define BUILDING_HPP

#include <SFML/Graphics.hpp>
#include <memory>
#include <string>

class Building {
public:
    Building(int id, float x, float y, const std::string& texturePath);
    Building(const Building& other); // Copy constructor

    int getId() const;
    std::string getTexturePath() const;
    void setPosition(float x, float y);
    sf::Vector2f getPosition() const;
    void draw(sf::RenderWindow& window) const;

    static const int BUILDING_WIDTH = 64;  // Adjust as needed
    static const int BUILDING_HEIGHT = 64; // Adjust as needed

private:
    int id;
    sf::Sprite sprite;
    std::string texturePath;
};

#endif // BUILDING_HPP