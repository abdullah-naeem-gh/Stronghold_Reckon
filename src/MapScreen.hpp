// MapScreen.hpp
#ifndef MAPSCREEN_HPP
#define MAPSCREEN_HPP

#include <SFML/Graphics.hpp>
#include "Map.hpp"
#include "UIManager.hpp"
#include "IsometricUtils.hpp" // Include IsometricUtils

class MapScreen {
public:
    MapScreen(int rows, int cols, const sf::Vector2u& windowSize);
    void handleEvents(const sf::Event& event, sf::RenderWindow& window);
    void draw(sf::RenderWindow& window);
    void setSelectedBuildingType(const std::string& buildingTexture);

private:
    Map mapEntity;
    UIManager uiManager;
    std::string selectedBuildingTexture; // Path to the selected building's texture
};

#endif // MAPSCREEN_HPP