// TankSpawn.hpp
#ifndef TANKSPAWN_HPP
#define TANKSPAWN_HPP

#include "Tank.hpp"
#include <vector>
#include <SFML/Graphics.hpp>
#include "Map.hpp"
#include "IsometricUtils.hpp" // Included to use TileCoordinates
#include "Pathfinding.hpp"
#include "Trap.hpp"

// Structure to hold tile coordinates
// struct TileCoordinates {
//     int row;
//     int col;
// };

class TankSpawn {
public:
    // Constructor initializes preset tiles and initializes the pathfinder with the provided map
    TankSpawn(const Map& map);

    // Handles events related to tank spawning
    void handleEvent(const sf::Event& event, Map& map);

    // Updates all active tanks and manages spawning logic
    void update(float deltaTime, Map& map);

    // Draws all active tanks on the window
    void draw(sf::RenderWindow& window, float deltaTime, Map& map);

    // Getter Methods
    const std::vector<std::shared_ptr<Tank>>& getTanks() const;

    // Removal Method
    // void removeDeadTanks();

private:
    std::vector<std::shared_ptr<Tank>> tanks; // Active tanks list
    Pathfinding pathFinder; // Pathfinding utility
    std::vector<TileCoordinates> presetTiles; // Predefined spawn locations

    // Coordinates of the town hall (destination for tanks)
    TileCoordinates townHall = {14, 14};

    // Spawns a tank on a randomly selected preset tile
    void spawnTankOnPresetTile(Map& map);

    size_t nextSpawnIndex; // Index for the next spawn
    float timeSinceLastSpawn; // Timer for spawn interval tracking
    bool spawningActive; // Flag indicating whether spawning should happen

    const float spawnInterval = 2.0f; // Time between spawns in seconds
};

#endif // TANKSPAWN_HPP