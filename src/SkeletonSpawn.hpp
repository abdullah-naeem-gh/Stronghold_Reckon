#ifndef SKELETONSPAWN_HPP
#define SKELETONSPAWN_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include "Skeleton.hpp"
#include "Map.hpp"
#include "Pathfinding.hpp"

class SkeletonSpawn {
public:
    SkeletonSpawn(const Map& map);
    void handleEvent(const sf::Event& event, Map& map);
    void update(float deltaTime, Map& map); // Update logic for timestep management
    void draw(sf::RenderWindow& window, float deltaTime);

private:
    std::vector<Skeleton> skeletons; // Active skeletons list
    std::vector<TileCoordinates> presetTiles; // Predefined spawn locations
    Pathfinding pathFinder; // Pathfinding utility
    TileCoordinates goal = {14, 14}; // Goal location to target

    const Map& map; // Reference to the map for current state
    bool spawningActive; // Flag indicating whether spawning should happen
    float timeSinceLastSpawn; // Timer for spawn interval tracking
    const float spawnInterval = 0.5f; // Time between spawns in seconds
    size_t nextSpawnIndex; // Index for the next spawn

    void spawnSkeleton(Map& map, const TileCoordinates& spawnLocation); // Utility to spawn a skeleton
};

#endif // SKELETONSPAWN_HPP