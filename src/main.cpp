// main.cpp
#include <SFML/Graphics.hpp>
#include "MapScreen.hpp"
#include "IsometricUtils.hpp"
#include <iostream>

int main() {
    const int WINDOW_WIDTH = 1280;
    const int WINDOW_HEIGHT = 720;
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Stronghold Reckoning");
    window.setFramerateLimit(60);

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
                default:
                    break;
                }
            }
            // Delegate handling to MapScreen
            mapScreen.handleEvents(event, window);
        }
        // Update camera position
        mapScreen.moveCamera(deltaTime);

        // Rendering
        window.clear(sf::Color::Black);
        mapScreen.draw(window, deltaSeconds);
        window.display();
    }
    return 0;
}