#include "Building.hpp"
   #include <iostream>

   const int TILE_WIDTH = 30;  // Adjust based on your tile size
   const int TILE_HEIGHT = 15; // Adjust based on your tile size

   // Initialize the static texture
   sf::Texture Building::buildingTexture;

   // Static method to load the texture once
   bool Building::loadTexture(const std::string& filePath) {
       if (!buildingTexture.loadFromFile(filePath)) {
           std::cerr << "Error: Could not load building texture from " << filePath << std::endl;
           return false;
       }
       std::cout << "Building texture loaded successfully!" << std::endl;
       sf::Vector2u textureSize = buildingTexture.getSize();
       std::cout << "Building texture size: " << textureSize.x << "x" << textureSize.y << std::endl;
       return true;
   }

   Building::Building() {
       buildingSprite.setTexture(buildingTexture);
       // Scale sprite to fit tile size
       sf::Vector2u textureSize = buildingTexture.getSize();
       buildingSprite.setScale(
           TILE_WIDTH / static_cast<float>(textureSize.x),
           TILE_HEIGHT / static_cast<float>(textureSize.y)
       );
   }

   void Building::setPosition(int row, int col) {
       int x = (col - row) * TILE_WIDTH / 2;
       int y = (col + row) * TILE_HEIGHT / 2;
       buildingSprite.setPosition(static_cast<float>(x), static_cast<float>(y));
   }

   void Building::draw(sf::RenderWindow& window) const {
       window.draw(buildingSprite);
   }