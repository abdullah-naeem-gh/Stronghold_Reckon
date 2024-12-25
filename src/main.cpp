// main.cpp
#include <SFML/Graphics.hpp>
#include "MapScreen.hpp"
#include "IsometricUtils.hpp"
#include "TextureManager.hpp" // **(1) Include the TextureManager header**
#include "GameState.hpp"
#include <iostream>

void checkGameEndCondition(sf::RenderWindow& window, int tanksAtTownHall, int skeletonsAtTownHall) {
    if (tanksAtTownHall >= 1 || skeletonsAtTownHall >= 10) {
        window.close();
        std::cout << "Game Over! " << (tanksAtTownHall >= 3 ? "3 Tanks" : "10 Skeletons") << " reached the town hall." << std::endl;
    }
}

int main() {
    const int WINDOW_WIDTH = 1280;
    const int WINDOW_HEIGHT = 720;
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Stronghold Reckoning");
    window.setFramerateLimit(60);

    // **(2) Load the sprite sheet before initializing MapScreen**
    TextureManager& tm = TextureManager::getInstance();
    std::string spriteSheetPath = "../assets/tiles/spritesheet.png"; // **Ensure this path is correct relative to the executable**
    if (!tm.loadSpriteSheet(spriteSheetPath)) { // **Attempt to load the sprite sheet**
        std::cerr << "Failed to load sprite sheet from: " << spriteSheetPath << std::endl;
        return -1; // **Exit if loading fails to prevent further errors**
    }
    std::cout << "Sprite sheet loaded successfully from: " << spriteSheetPath << std::endl;
    
    // Initialize MapScreen with desired dimensions
    // For example, a 30x30 map
    MapScreen mapScreen(30, 30, window.getSize());
    sf::Clock deltaClock;

    
    
    while (window.isOpen()) {
        sf::Time deltaTime = deltaClock.restart();
        float deltaSeconds = deltaTime.asSeconds(); // Convert sf::Time to float
        sf::Event event;
        
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                std::cout << "closed event? " << std::endl;
                window.close();
            }
            else if (event.type == sf::Event::KeyPressed) {
                switch (event.key.code) {
                    case sf::Keyboard::O:
                        mapScreen.saveMap("savemap.txt");
                        break;
                    case sf::Keyboard::L:
                        mapScreen.loadMap("savemap.txt");
                        break;
                    case sf::Keyboard::Z:
                        mapScreen.getMapEntity().undo(); // Handle undo
                        break;
                    case sf::Keyboard::Y:
                        mapScreen.getMapEntity().redo(); // Handle redo
                        break;

                    case sf::Keyboard::M:
                        window.close();
                        break;
                    default:
                        break;
                }
            }
            // Delegate event handling to MapScreen
            mapScreen.handleEvents(event, window);
            checkGameEndCondition(window, tanksAtTownHall, skeletonsAtTownHall);
        }
        
        // Update camera position
        mapScreen.moveCamera(deltaTime);
        mapScreen.update(deltaSeconds); // Update logic for map screen including skeleton
        
        // Rendering
        window.clear(sf::Color::Black);
        mapScreen.draw(window, deltaSeconds);
        window.display();
    }
    
    return 0;
}