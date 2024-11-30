// main.cpp
#include <SFML/Graphics.hpp>
#include "MapScreen.hpp"

int main() {
    const int WINDOW_WIDTH = 1280;
    const int WINDOW_HEIGHT = 720;
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Stronghold Reckoning");
    window.setFramerateLimit(60);

    // Initialize MapScreen with 20 rows x 20 columns
    MapScreen mapScreen(20, 20, window.getSize());

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            mapScreen.handleEvents(event, window);
        }

        window.clear(sf::Color::Black);
        mapScreen.draw(window);
        window.display();
    }

    return 0;
}