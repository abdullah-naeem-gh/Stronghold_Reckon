#ifndef SKELETONSPAWN_HPP
#define SKELETONSPAWN_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include "Skeleton.hpp"
#include "Map.hpp"
#include "Pathfinding.hpp"
#include "IsometricUtils.hpp"

class SkeletonSpawn {
public:
    SkeletonSpawn(const Map& map);
    void handleEvent(const sf::Event& event, Map& map);
    void update(float deltaTime, Map& map);
    void draw(sf::RenderWindow& window, float deltaTime);

    const std::vector<std::unique_ptr<Skeleton>>& getSkeletons() const;
    std::vector<std::unique_ptr<Skeleton>>& getSkeletons();

    void removeDeadSkeletons();

private:
    std::vector<std::unique_ptr<Skeleton>> skeletons;
    std::vector<TileCoordinates> presetTiles;
    Pathfinding pathFinder;
    TileCoordinates goal = {14, 14};
    bool spawningActive;
    float timeSinceLastSpawn;
    const float spawnInterval = 0.5f;
    size_t nextSpawnIndex;

    // Reference to the map is no longer required as a member here since it’s passed directly to Skeleton

    void spawnSkeleton(Map& map, const TileCoordinates& spawnLocation);
};

#endif // SKELETONSPAWN_HPP