// MapScreen.hpp
#ifndef MAPSCREEN_HPP
#define MAPSCREEN_HPP

#include <SFML/Graphics.hpp>
#include "Map.hpp"
#include "UIManager.hpp"
#include "TankSpawn.hpp"

class MapScreen {
public:
    MapScreen(int rows, int cols, const sf::Vector2u& windowSize);
    
    // Handles all events related to the map screen
    void handleEvents(const sf::Event& event, sf::RenderWindow& window);
    
    // Draws the map screen elements
    void draw(sf::RenderWindow& window, float deltaTime);
    
    // Sets the currently selected building type for placement
    void setSelectedBuildingType(const std::string& buildingTexture);
    
    // Moves the camera based on user input and elapsed time
    void moveCamera(const sf::Time& deltaTime);
    
    // Saves the current map state to a file
    void saveMap(const std::string& filename);
    
    // Loads the map state from a file
    void loadMap(const std::string& filename);
    
    // Provides access to the map entity for actions like undo/redo
    Map& getMapEntity();

private:
    Map mapEntity;         // The map containing tiles and buildings
    UIManager uiManager;  // Manages UI elements
    std::string selectedBuildingTexture; // Currently selected building texture
    sf::View cameraView;  // Camera view for the map
    float cameraSpeed = 300.0f; // Speed at which the camera moves

    TankSpawn tankSpawn; // Handles tank spawning mechanics
    // std::string selectedBuildingTexture;
    TileType selectedTileType;
};

#endif // MAPSCREEN_HPP