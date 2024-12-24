// Pathfinding.cpp
#include "Pathfinding.hpp"
#include "Tile.hpp"
#include "Map.hpp"
#include <vector>
#include <memory>
#include <cmath>
#include <iostream>

Pathfinding::Pathfinding(const Map& map) : map(map) {}

std::vector<std::shared_ptr<Tile>> Pathfinding::findPath(
    std::shared_ptr<Tile> start, 
    std::shared_ptr<Tile> end, 
    int stopBeforeWallTiles
) {
    std::vector<std::shared_ptr<Tile>> path;
    std::shared_ptr<Tile> current = start;

    while (current != end) {
        // Get the next tile in the straight path towards the end
        auto nextTile = getNextTileInStraightPath(current, end);
        if (!nextTile) {
            break; // No valid next tile
        }

        // Check if the next tile is blocked
        if (nextTile->isBlocked()) {
            // Calculate the stopping tile based on the given parameter
            auto stopTile = getTileStepsBeforeWall(current, nextTile, stopBeforeWallTiles);
            if (stopTile) {
                path.push_back(stopTile);
            }

            // **Ensure that when stopBeforeWallTiles is 0, the wall tile itself is included**
            if (stopBeforeWallTiles <= 0) {
                path.push_back(nextTile); // Include the wall tile
            }

            return path; // Stop pathfinding upon reaching this condition
        }

        path.push_back(current);
        current = nextTile;
    }

    path.push_back(end);
    return path;
}

std::shared_ptr<Tile> Pathfinding::getTileStepsBeforeWall(
    std::shared_ptr<Tile> current, 
    std::shared_ptr<Tile> wallTile, 
    int stopBeforeWallTiles
) {
    if (stopBeforeWallTiles <= 0) {
        // When stopBeforeWallTiles is 0, return the wallTile itself
        return wallTile;
    }

    int dx = wallTile->getRow() - current->getRow();
    int dy = wallTile->getCol() - current->getCol();
    int stepX = (dx != 0) ? (dx / std::abs(dx)) : 0;
    int stepY = (dy != 0) ? (dy / std::abs(dy)) : 0;

    return current->getNeighbor(-stopBeforeWallTiles * stepX, -stopBeforeWallTiles * stepY);
}

std::shared_ptr<Tile> Pathfinding::getNextTileInStraightPath(std::shared_ptr<Tile> current, std::shared_ptr<Tile> end) {
    int dx = end->getRow() - current->getRow();
    int dy = end->getCol() - current->getCol();
    int stepX = (dx != 0) ? (dx / std::abs(dx)) : 0;
    int stepY = (dy != 0) ? (dy / std::abs(dy)) : 0;
    return current->getNeighbor(stepX, stepY);
}

std::shared_ptr<Tile> Pathfinding::getTileTwoStepsBehind(std::shared_ptr<Tile> current, std::shared_ptr<Tile> wallTile) {
    int dx = wallTile->getRow() - current->getRow();
    int dy = wallTile->getCol() - current->getCol();
    int stepX = (dx != 0) ? (dx / std::abs(dx)) : 0;
    int stepY = (dy != 0) ? (dy / std::abs(dy)) : 0;
    return current->getNeighbor(-2 * stepX, -2 * stepY);
}

void Pathfinding::attackWall(std::shared_ptr<Tile> wallTile) {
    // Implement wall attacking logic
    wallTile->takeDamage(10.0f); // Example: Deal 10 damage
    // Optionally remove the wall if damage exceeds threshold
}