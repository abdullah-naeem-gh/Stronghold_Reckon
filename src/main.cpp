#include <SFML/Graphics.hpp>
#include "MapScreen.hpp"
#include <iostream> // For debug output

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Stronghold Reckoning");
    MapScreen mapScreen;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {

            // Check if window close event is detected
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            // Handle events through map screen logic
            mapScreen.handleEvents(event);
        }

        // Draw the map screen and handle all rendering
        mapScreen.draw(window);
    }

    return 0;
}