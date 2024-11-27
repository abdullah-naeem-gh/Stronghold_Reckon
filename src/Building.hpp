#ifndef BUILDING_HPP
   #define BUILDING_HPP

   #include <SFML/Graphics.hpp>

   class Building {
   public:
       Building();
       Building(const Building&) = delete; // Prevent copying
       Building& operator=(const Building&) = delete; // Prevent assignment

       // Optionally, make it movable
       Building(Building&&) noexcept = default;
       Building& operator=(Building&&) noexcept = default;

       void setPosition(int row, int col);
       void draw(sf::RenderWindow& window) const;

       // Function to ensure texture is loaded only once
       static bool loadTexture(const std::string& filePath);

   private:
       static sf::Texture buildingTexture;
       sf::Sprite buildingSprite;
   };

   #endif // BUILDING_HPP