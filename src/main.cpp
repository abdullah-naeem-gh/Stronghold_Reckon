#include <SFML/Graphics.hpp>
#include "MapScreen.hpp"

int main() {
    const int WINDOW_WIDTH = 1280;
    const int WINDOW_HEIGHT = 720;
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Stronghold Reckoning");
    window.setFramerateLimit(60);

    // Initialize MapScreen with 100 rows x 100 columns for a large map
    MapScreen mapScreen(100, 100, window.getSize());
    sf::Clock deltaClock;

    while (window.isOpen()) {
        sf::Time deltaTime = deltaClock.restart();
        sf::Event event;

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            } else if (event.type == sf::Event::KeyPressed) {
                switch (event.key.code) {
                    case sf::Keyboard::O:
                        mapScreen.saveMap("savemap.txt");
                        break;
                    case sf::Keyboard::L:
                        mapScreen.loadMap("savemap.txt");
                        break;
                    case sf::Keyboard::Z:
                        mapScreen.getMapEntity().undo();  // Handling undo in main
                        break;
                    case sf::Keyboard::Y:
                        mapScreen.getMapEntity().redo();  // Handling redo in main
                        break;
                    default:
                        break;
                }
            }
            mapScreen.handleEvents(event, window);
        }
        
        mapScreen.moveCamera(deltaTime);
        window.clear(sf::Color::Black);
        mapScreen.draw(window);
        window.display();
    }

    return 0;
}