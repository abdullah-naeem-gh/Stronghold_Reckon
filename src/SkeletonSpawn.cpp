#include "SkeletonSpawn.hpp"
#include "IsometricUtils.hpp"
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <algorithm> 
#include <random>  

using namespace std;

SkeletonSpawn::SkeletonSpawn(const Map& map)
    : map(map), pathFinder(map), nextSpawnIndex(0), timeSinceLastSpawn(0.0f), spawningActive(false) {
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    int rows = map.getRows();
    int cols = map.getCols();

    // Populate presetTiles with all boundary tiles
    for (int i = 0; i < rows; ++i) {
        presetTiles.push_back({i, 0}); // Left boundary
        presetTiles.push_back({i, cols - 1}); // Right boundary
    }
    for (int j = 0; j < cols; ++j) {
        presetTiles.push_back({0, j}); // Top boundary
        presetTiles.push_back({rows - 1, j}); // Bottom boundary
    }
}

void SkeletonSpawn::handleEvent(const sf::Event& event, Map& map) {
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

        // Update the position of all active skeletons
        for (auto& skeleton : skeletons) {
            skeleton.move(deltaTime);
        }
    }
}

void SkeletonSpawn::spawnSkeleton(Map& map, const TileCoordinates& spawnLocation) {
    auto tile = map.getTile(spawnLocation.row, spawnLocation.col);
    if (!tile || tile->getBuilding()) {
        std::cerr << "Invalid or occupied tile at (" << spawnLocation.row << ", " << spawnLocation.col << ").\n";
        return;
    }
    
    // Position the skeleton based on tile coordinates
    sf::Vector2f isoPos = IsometricUtils::tileToScreen(spawnLocation.row, spawnLocation.col);
    sf::Vector2f skeletonPosition = sf::Vector2f(isoPos.x + Tile::TILE_WIDTH / 2.0f, isoPos.y + Tile::TILE_HEIGHT);
    
    // Find path from spawn location to goal
    std::vector<std::shared_ptr<Tile>> path = pathFinder.findPath(
        map.getTile(spawnLocation.row, spawnLocation.col),
        map.getTile(goal.row, goal.col)
    );

    if (path.empty()) {
        std::cerr << "No available path from (" << spawnLocation.row << ", " << spawnLocation.col
                  << ") to (" << goal.row << ", " << goal.col << ").\n";
        return;
    }

    // Add skeleton to the list
    skeletons.emplace_back(skeletonPosition.x, skeletonPosition.y, path);
    std::cout << "Skeleton placed at tile: (" << spawnLocation.row << ", " << spawnLocation.col << ").\n";
}

void SkeletonSpawn::draw(sf::RenderWindow& window, float deltaTime) {
    for (auto& skeleton : skeletons) {
        skeleton.draw(window, deltaTime);
    }
}