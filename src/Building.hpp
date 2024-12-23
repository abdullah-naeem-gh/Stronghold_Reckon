// Building.hpp
#ifndef BUILDING_HPP
#define BUILDING_HPP

#include <SFML/Graphics.hpp>
#include <memory>
#include <string>
#include "IsometricUtils.hpp"

// Forward declaration of TextureManager
class TextureManager;

class Building {
public:
    // Building(int id, float x, float y, const std::string& texturePath);
    Building(int id, int row, int col, const std::string& texturePath);


    Building(const Building& other); // Copy constructor
    
    int getId() const;
    std::string getTexturePath() const;
    void setPosition(float x, float y);
    sf::Vector2f getPosition() const;
    void draw(sf::RenderWindow& window, float x, float y);
    
    static const int BUILDING_WIDTH = 64;  // Adjust as needed
    static const int BUILDING_HEIGHT = 64; // Adjust as needed

private:
    int id;
    sf::Sprite sprite;
    std::string texturePath;

    int row;
    int col;
};

#endif // BUILDING_HPP