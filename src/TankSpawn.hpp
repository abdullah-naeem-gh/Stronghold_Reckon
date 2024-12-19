// TankSpawn.hpp
#ifndef TANKSPAWN_HPP
#define TANKSPAWN_HPP

#include "Tank.hpp"
#include <vector>
#include <SFML/Graphics.hpp>
#include "Map.hpp"
#include "IsometricUtils.hpp"
#include "Pathfinding.hpp"
#include <iostream>

class TankSpawn {
public:
    // Constructor initializes preset tiles and pathfinder with the provided map
    TankSpawn(const Map& map);
    
    // Handles events related to tank spawning
    void handleEvent(const sf::Event& event, Map& map);
    
    // Draws all active tanks on the window
    void draw(sf::RenderWindow& window, float deltaTime);
    
    // Returns a const reference to the vector of tanks for external access if needed
    const std::vector<Tank>& getTanks() const;

private:
    std::vector<Tank> tanks; // Stores all active tanks
    
    // Pathfinding instance to compute paths for tanks
    Pathfinding pathFinder;
    
    // Preset tiles where tanks can spawn
    std::vector<TileCoordinates> presetTiles;
    
    // Path to the tank texture asset
    const std::string TANK_TEXTURE_PATH = "../assets/enemies/tank/tank_left.png";
    
    // Coordinates of the town hall (destination for tanks)

    TileCoordinates townHall= {14, 14};
    
    // Spawns a tank on a randomly selected preset tile
    
    void spawnTankOnPresetTile(Map& map);
};

#endif // TANKSPAWN_HPP