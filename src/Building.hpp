#ifndef BUILDING_HPP
#define BUILDING_HPP

#include <SFML/Graphics.hpp>
#include <memory>
#include <string>

class Building {
public:
    Building(int id, float x, float y, const std::string& texturePath);
    int getId() const;
    void setPosition(float x, float y);
    sf::Vector2f getPosition() const;
    void draw(sf::RenderWindow& window) const;

    // Define constants for building dimensions
    static const int BUILDING_WIDTH = 64;  // Adjust based on your texture size
    static const int BUILDING_HEIGHT = 64; // Adjust based on your texture size
    std::string getTexturePath() const; // Function to get texture path

private:
    int id;
    sf::Sprite sprite;
    std::string texturePath; // Declare texturePath here

};

#endif // BUILDING_HPP