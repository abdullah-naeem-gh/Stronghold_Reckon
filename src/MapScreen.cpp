// MapScreen.cpp
   #include "MapScreen.hpp"
   #include <iostream>
   #include <memory> // Include memory for std::unique_ptr

   const int TILE_WIDTH = 30;  // Adjust based on your tile size
   const int TILE_HEIGHT = 15; // Adjust based on your tile size

   MapScreen::MapScreen() {
       if (!Building::loadTexture("../assets/building1.png")) {
           std::cerr << "Failed to load building texture." << std::endl;
           // Handle texture loading failure if necessary
       }
   }

   void MapScreen::handleEvents(const sf::Event& event) {
       if (event.type == sf::Event::MouseButtonPressed) {
           std::cout << "Mouse button pressed event detected" << std::endl;
           if (event.mouseButton.button == sf::Mouse::Left) {
               std::cout << "Left mouse button detected" << std::endl;
               int mouseX = event.mouseButton.x;
               int mouseY = event.mouseButton.y;
               int col = ((mouseX / (TILE_WIDTH / 2)) + (mouseY / (TILE_HEIGHT / 2))) / 2;
               int row = ((mouseY / (TILE_HEIGHT / 2)) - (mouseX / (TILE_WIDTH / 2))) / 2;
               std::cout << "Mouse Click at: (" << mouseX << ", " << mouseY << ")" << std::endl;
               std::cout << "Calculated Tile Position: Row " << row << ", Col " << col << std::endl;

               // Create a unique_ptr to a new Building
               std::unique_ptr<Building> building(new Building());
               building->setPosition(row, col);
               buildings.push_back(std::move(building));
               std::cout << "Building added at Row: " << row << ", Col: " << col << std::endl;
           }
       }
   }

   void MapScreen::draw(sf::RenderWindow& window) {
       window.clear(sf::Color::Black);
       mapEntity.draw(window);
       for (const auto& building : buildings) {
           building->draw(window);
       }
       window.display();
   }