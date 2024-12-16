#ifndef MAPSCREEN_HPP
#define MAPSCREEN_HPP

#include <SFML/Graphics.hpp>
#include "Map.hpp"
#include "UIManager.hpp"
#include "TankSpawn.hpp"

class MapScreen {
public:
    MapScreen(int rows, int cols, const sf::Vector2u& windowSize);
    void handleEvents(const sf::Event& event, sf::RenderWindow& window);
    void draw(sf::RenderWindow& window);
    void setSelectedBuildingType(const std::string& buildingTexture);
    void moveCamera(const sf::Time& deltaTime);
    void saveMap(const std::string &filename);
    void loadMap(const std::string &filename);
    Map& getMapEntity(); // Method to access Map for actions like undo/redo etc.

private:
    Map mapEntity;
    UIManager uiManager;
    std::string selectedBuildingTexture;
    sf::View cameraView;
    float cameraSpeed = 300.0f; // Speed at which the camera moves
    static const int START_X = 400;
    static const int START_Y = 50;
    TankSpawn tankSpawn; // Moved inside MapScreen
};

#endif // MAPSCREEN_HPP