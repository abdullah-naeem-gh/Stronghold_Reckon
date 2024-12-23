// SkeletonSpawn.hpp
#ifndef SKELETONSPAWN_HPP
#define SKELETONSPAWN_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include "Skeleton.hpp"
#include "Map.hpp"
#include "Pathfinding.hpp"
#include "IsometricUtils.hpp" // Include to access TileCoordinates

class SkeletonSpawn {
public:
    SkeletonSpawn(const Map& map);
    void handleEvent(const sf::Event& event, Map& map);
    void update(float deltaTime, Map& map); // Update logic for timestep management
    void draw(sf::RenderWindow& window, float deltaTime);
    // **Getter Methods**
    const std::vector<std::unique_ptr<Skeleton>>& getSkeletons() const;
    std::vector<std::unique_ptr<Skeleton>>& getSkeletons(); // Non-const getter
    // **Removal Method**
    void removeDeadSkeletons();
private:
    std::vector<std::unique_ptr<Skeleton>> skeletons; // Active skeletons list
    std::vector<TileCoordinates> presetTiles; // Predefined spawn locations
    Pathfinding pathFinder; // Pathfinding utility
    TileCoordinates goal = {14, 14}; // Goal location to target
    bool spawningActive; // Flag indicating whether spawning should happen
    float timeSinceLastSpawn; // Timer for spawn interval tracking
    const float spawnInterval = 0.5f; // Time between spawns in seconds
    size_t nextSpawnIndex; // Index for the next spawn
    void spawnSkeleton(Map& map, const TileCoordinates& spawnLocation); // Utility to spawn a skeleton
};

#endif // SKELETONSPAWN_HPP