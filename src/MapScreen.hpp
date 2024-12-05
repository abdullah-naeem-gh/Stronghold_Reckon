#ifndef MAPSCREEN_HPP
#define MAPSCREEN_HPP

#include <SFML/Graphics.hpp>
#include "Map.hpp"
#include "UIManager.hpp"
#include "IsometricUtils.hpp"
#include "Player.hpp"

class MapScreen {
public:
    MapScreen(int rows, int cols, const sf::Vector2u& windowSize);
    void handleEvents(const sf::Event& event, sf::RenderWindow& window);
    void draw(sf::RenderWindow& window);
    void setSelectedBuildingType(const std::string& buildingTexture);
    void moveCamera(const sf::Time& deltaTime);
    void saveMap(const std::string &filename);
    void loadMap(const std::string &filename);
    Map& getMapEntity() { return mapEntity; }

    void initializePath(); // New method to initialize player pathfinding

private:
    Map mapEntity;
    UIManager uiManager;
    Player player;
    std::string selectedBuildingTexture;
    sf::View cameraView;
    float cameraSpeed = 300.0f;
    std::shared_ptr<Tile> selectedTile;
    std::shared_ptr<Tile> highlightTile1;
    std::shared_ptr<Tile> highlightTile2;

    std::shared_ptr<Tile> findTownhallTile(); // New helper method

    std::vector<std::shared_ptr<Tile>> pathfind(std::shared_ptr<Tile> start, std::shared_ptr<Tile> goal, const Map &map);

    static constexpr int playerInitialRow = 0; // Assuming row 0 for player initial spawn
};

#endif // MAPSCREEN_HPP