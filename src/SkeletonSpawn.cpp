// SkeletonSpawn.cpp
#include "SkeletonSpawn.hpp"
#include "IsometricUtils.hpp"
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <algorithm>
#include <random>

// Constructor
// Constructor
SkeletonSpawn::SkeletonSpawn(const Map& map)
    : pathFinder(map), nextSpawnIndex(0), timeSinceLastSpawn(0.0f), spawningActive(false) {
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    int rows = map.getRows();
    int cols = map.getCols();

    // Populate presetTiles with all boundary tiles
    for (int i = 0; i < rows; ++i) {
        presetTiles.emplace_back(TileCoordinates{i, 0}); // Left boundary
        presetTiles.emplace_back(TileCoordinates{i, cols - 1}); // Right boundary
    }
    for (int j = 0; j < cols; ++j) {
        presetTiles.emplace_back(TileCoordinates{0, j}); // Top boundary
        presetTiles.emplace_back(TileCoordinates{rows - 1, j}); // Bottom boundary
    }
}

void SkeletonSpawn::handleEvent(const sf::Event& event, Map& /*map*/) {
    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::I) {
        spawningActive = true;
        nextSpawnIndex = 0;
        timeSinceLastSpawn = 0.0f;
        // Create a random device and a random number generator
        std::random_device rd;
        std::mt19937 g(rd());
        // Shuffle presetTiles with the random generator
        std::shuffle(presetTiles.begin(), presetTiles.end(), g);
    }
}

void SkeletonSpawn::spawnSkeleton(Map& map, const TileCoordinates& spawnLocation) {
    auto tile = map.getTile(spawnLocation.row, spawnLocation.col);
    if (!tile || tile->getBuilding()) {
        std::cerr << "Invalid or occupied tile at (" << spawnLocation.row << ", " << spawnLocation.col << ").\n";
        return;
    }

    sf::Vector2f isoPos = IsometricUtils::tileToScreen(spawnLocation.row, spawnLocation.col);
    sf::Vector2f skeletonPosition = sf::Vector2f(isoPos.x + Tile::TILE_WIDTH / 2.0f, isoPos.y + Tile::TILE_HEIGHT);

    // Find path from spawn location to goal
    std::vector<std::shared_ptr<Tile>> path = pathFinder.findPath(
        map.getTile(spawnLocation.row, spawnLocation.col),
        map.getTile(goal.row, goal.col),
        0
    );

    if (path.empty()) {
        std::cerr << "No available path from (" << spawnLocation.row << ", " << spawnLocation.col
                  << ") to (" << goal.row << ", " << goal.col << ").\n";
        return;
    }

    // Create a skeleton with the map reference
    skeletons.emplace_back(std::make_unique<Skeleton>(skeletonPosition.x, skeletonPosition.y, path, map));

    // std::cout << "Skeleton placed at tile: (" << spawnLocation.row << ", " << spawnLocation.col << ").\n";
}

void SkeletonSpawn::draw(sf::RenderWindow& window, float deltaTime) {
    for (auto& skeleton : skeletons) {
        skeleton->draw(window, deltaTime);
    }
}

const std::vector<std::unique_ptr<Skeleton>>& SkeletonSpawn::getSkeletons() const {
    return skeletons;
}

std::vector<std::unique_ptr<Skeleton>>& SkeletonSpawn::getSkeletons() {
    return skeletons;
}

void SkeletonSpawn::removeDeadSkeletons() {
    size_t initialSize = skeletons.size();
    skeletons.erase(
        std::remove_if(skeletons.begin(), skeletons.end(),
            [](const std::unique_ptr<Skeleton>& s) { return !s->isAlive(); }),
        skeletons.end()
    );
    size_t finalSize = skeletons.size();
}

void SkeletonSpawn::update(float deltaTime, Map& map) {
    if (spawningActive) {
        timeSinceLastSpawn += deltaTime; // Increment time with each frame
        if (nextSpawnIndex < presetTiles.size() && timeSinceLastSpawn >= spawnInterval) {
            // It's time to spawn a new skeleton
            spawnSkeleton(map, presetTiles[nextSpawnIndex]);
            nextSpawnIndex++;
            timeSinceLastSpawn = 0.0f; // Reset spawn timer
            if (nextSpawnIndex >= presetTiles.size()) {
                spawningActive = false; // Stop spawning once all have spawned
            }
        }
    }
    // Update the position of all active skeletons
    for (auto& skeleton : skeletons) {
        skeleton->move(deltaTime);
    }
}