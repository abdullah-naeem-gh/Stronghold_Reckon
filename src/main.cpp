#include <SFML/Graphics.hpp>
#include "MapScreen.hpp"
#include "IsometricUtils.hpp"

int main() {
    const int WINDOW_WIDTH = 1280;
    const int WINDOW_HEIGHT = 720;
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Stronghold Reckoning");
    window.setFramerateLimit(60);
    
    // Initialize MapScreen with 64 rows x 64 columns
    MapScreen mapScreen(64, 64, window.getSize());
    
    // Initialize camera view
    sf::View cameraView(sf::FloatRect(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT));
    sf::Vector2f centerPosition = IsometricUtils::tileToScreen(mapScreen.getMapEntity().getRows() / 2,
                                                               mapScreen.getMapEntity().getCols() / 2,
                                                               0,
                                                               0);
    cameraView.setCenter(centerPosition);
    
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
        window.setView(cameraView);
        mapScreen.draw(window);
        window.display();
    }
    return 0;
}