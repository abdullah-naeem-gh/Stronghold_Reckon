#ifndef MAPSCREEN_HPP
#define MAPSCREEN_HPP

#include <SFML/Graphics.hpp>
#include "Map.hpp"
#include "UIManager.hpp"
#include "TankSpawn.hpp"
#include "SkeletonSpawn.hpp"
#include "BulletManager.hpp"
#include "Tower.hpp" // Include Tower class
#include "Tank.hpp"
#include "Trap.hpp"

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
    // void handleBulletCollisions();

    void handleBulletCollisions(float deltatime);
    void setSelectedTrapType(const std::string& trapTexture);

    
private:
    Map mapEntity;
    UIManager uiManager;
    std::string selectedBuildingTexture;
    sf::View cameraView;
    float cameraSpeed = 300.0f;
    TankSpawn tankSpawn;
    SkeletonSpawn skeletonSpawn;
    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;
    BulletManager centralBulletManager; // Central BulletManager

    // traps
    std::string selectedTrapTexture; // Add a member variable for the selected trap texture
    void initializeTowers(); // Initialize towers and pass central BulletManager
};

#endif // MAPSCREEN_HPP