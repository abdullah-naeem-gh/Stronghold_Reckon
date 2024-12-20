#ifndef MAPSCREEN_HPP
#define MAPSCREEN_HPP

#include <SFML/Graphics.hpp>
#include "Map.hpp"
#include "UIManager.hpp"
#include "TankSpawn.hpp"
#include "SkeletonSpawn.hpp" // Include SkeletonSpawn header

class MapScreen {
public:
    MapScreen(int rows, int cols, const sf::Vector2u& windowSize);
    void handleEvents(const sf::Event& event, sf::RenderWindow& window);
    void draw(sf::RenderWindow& window, float deltaTime);
    void setSelectedBuildingType(const std::string& buildingTexture);
    void moveCamera(const sf::Time& deltaTime);
    void saveMap(const std::string& filename);
    void loadMap(const std::string& filename);
    Map& getMapEntity();

    void update(float deltaTime);

private:
    Map mapEntity;
    UIManager uiManager;
    std::string selectedBuildingTexture;
    sf::View cameraView;
    float cameraSpeed = 300.0f;
    TankSpawn tankSpawn;
    SkeletonSpawn skeletonSpawn; // Add SkeletonSpawn instance
    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;
};

#endif // MAPSCREEN_HPP