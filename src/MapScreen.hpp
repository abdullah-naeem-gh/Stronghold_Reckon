#ifndef MAPSCREEN_HPP
   #define MAPSCREEN_HPP

   #include <SFML/Graphics.hpp>
   #include "Map.hpp"
   #include "Building.hpp"
   #include <vector>
   #include <memory> // Include memory for smart pointers

   class MapScreen {
   public:
       MapScreen();
       void handleEvents(const sf::Event& event);
       void draw(sf::RenderWindow &window);
   private:
       Map mapEntity;
       std::vector<std::unique_ptr<Building>> buildings; // Use smart pointers
   };

   #endif // MAPSCREEN_HPP