#ifndef MAPSCREEN_HPP
#define MAPSCREEN_HPP

#include <SFML/Graphics.hpp>
#include "Map.hpp"
#include "UIManager.hpp"
#include "IsometricUtils.hpp"

class MapScreen {
public:
    MapScreen(int rows, int cols, const sf::Vector2u& windowSize);
    void handleEvents(const sf::Event& event, sf::RenderWindow& window);
    void draw(sf::RenderWindow& window);
    void setSelectedBuildingType(const std::string& buildingTexture);
    void moveCamera(const sf::Time& deltaTime);

    void saveMap(const std::string &filename);

    void loadMap(const std::string &filename);

private:
    Map mapEntity;
    UIManager uiManager;
    std::string selectedBuildingTexture;
    sf::View cameraView;
    float cameraSpeed = 300.0f;

    std::shared_ptr<Tile> selectedTile;    // Current tile selection
    std::shared_ptr<Tile> highlightTile1;  // First highlighted tile
    std::shared_ptr<Tile> highlightTile2;  // Second highlighted tile
};

#endif // MAPSCREEN_HPP