// TankSpawn.cpp
#include "TankSpawn.hpp"
#include "IsometricUtils.hpp"
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <algorithm>
#include <random>

// Constructor initializes preset tiles and initializes the pathfinder with the provided map
TankSpawn::TankSpawn(const Map& map)
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


// Handles events related to tank spawning
void TankSpawn::handleEvent(const sf::Event& event, Map& map) {
    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::P) {
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

// Spawns a tank on a randomly selected preset tile
void TankSpawn::spawnTankOnPresetTile(Map& map) {
    if (!presetTiles.empty()) {
        TileCoordinates spawnLocation = presetTiles[nextSpawnIndex];
        auto tile = map.getTile(spawnLocation.row, spawnLocation.col);
        if (!tile || tile->getBuilding()) {
            std::cerr << "Invalid or occupied tile at (" << spawnLocation.row << ", " << spawnLocation.col << ").\n";
            return;
        }
        sf::Vector2f isoPos = IsometricUtils::tileToScreen(spawnLocation.row, spawnLocation.col);
        sf::Vector2f tankPosition = sf::Vector2f(isoPos.x, isoPos.y + Tile::TILE_HEIGHT);

        // Pathfinding from spawn location to the town hall.
        std::vector<std::shared_ptr<Tile>> path = pathFinder.findPath(
            map.getTile(spawnLocation.row, spawnLocation.col),
            map.getTile(townHall.row, townHall.col)
        );

        if (path.empty()) {
            std::cerr << "No available path from (" << spawnLocation.row << ", " << spawnLocation.col
                      << ") to (" << townHall.row << ", " << townHall.col << ").\n";
            return;
        }
        tanks.emplace_back(std::make_shared<Tank>(tankPosition.x, tankPosition.y, path));
        std::cout << "Tank placed at tile: (" << spawnLocation.row << ", " << spawnLocation.col << ").\n";
    } else {
        std::cerr << "No preset tiles available for spawning tanks.\n";
    }
}

// Updates all active tanks and manages spawning logic
void TankSpawn::update(float deltaTime, Map& map) {
    if (spawningActive) {
        timeSinceLastSpawn += deltaTime; // Increment time with each frame
        if (nextSpawnIndex < presetTiles.size() && timeSinceLastSpawn >= spawnInterval) {
            // It's time to spawn a new tank
            spawnTankOnPresetTile(map);
            nextSpawnIndex++;
            timeSinceLastSpawn = 0.0f; // Reset spawn timer
            if (nextSpawnIndex >= presetTiles.size()) {
                spawningActive = false; // Stop spawning once all have spawned
            }
        }
    }

    // Update the tanks
    for (auto& tank : tanks) {
        tank->update(deltaTime); // Correctly call the update method
    }
}

// Draws all active tanks
void TankSpawn::draw(sf::RenderWindow& window, float deltaTime, Map& map) {
    for (auto& tank : tanks) {
        tank->draw(window);
    }
}

// Returns a const reference to the tanks vector
const std::vector<std::shared_ptr<Tank>>& TankSpawn::getTanks() const {
    return tanks;
}

// Removes dead tanks
void TankSpawn::removeDeadTanks() {
    size_t initialSize = tanks.size();
    tanks.erase(
        std::remove_if(tanks.begin(), tanks.end(),
            [](const std::shared_ptr<Tank>& t) { return !t->isAlive(); }),
        tanks.end()
    );
    size_t finalSize = tanks.size();
    if (initialSize != finalSize) {
        std::cout << "Removed " << (initialSize - finalSize) << " dead tank(s).\n";
    }
}