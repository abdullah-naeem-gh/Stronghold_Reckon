#include "Pathfinding.hpp"
#include "Tile.hpp"
#include "Map.hpp"
#include <vector>
#include <memory>
#include <cmath>

Pathfinding::Pathfinding(const Map& map) : map(map) {}

 std::vector<std::shared_ptr<Tile>> Pathfinding::findPath(std::shared_ptr<Tile> start, std::shared_ptr<Tile> end) {
        std::vector<std::shared_ptr<Tile>> path;
        std::shared_ptr<Tile> current = start;
        while (current != end) {
            // Get the next tile in the straight path towards the end
            auto nextTile = getNextTileInStraightPath(current, end);
            if (!nextTile) {
                break; // No valid next tile
            }
            
            // Check if the next tile contains a wall
            if (nextTile->getBuilding() && nextTile->getBuilding()->getTexturePath() == "../assets/walls/brick_wall.png") {
                // Stop 2 tiles behind the wall
                auto stopTile = getTileTwoStepsBehind(current, nextTile);
                if (stopTile) {
                    path.push_back(stopTile);
                }
                attackWall(nextTile);
                return path; // Stop pathfinding after encountering a wall
            }

            path.push_back(current);
            current = nextTile;
        }
        path.push_back(end);
        return path;
    }

std::shared_ptr<Tile> Pathfinding::getNextTileInStraightPath(std::shared_ptr<Tile> current, std::shared_ptr<Tile> end) {
    int dx = end->getRow() - current->getRow();
    int dy = end->getCol() - current->getCol();

    int stepX = (dx != 0) ? dx / std::abs(dx) : 0;
    int stepY = (dy != 0) ? dy / std::abs(dy) : 0;

    return current->getNeighbor(stepX, stepY);
}

std::shared_ptr<Tile> Pathfinding::getTileTwoStepsBehind(std::shared_ptr<Tile> current, std::shared_ptr<Tile> wallTile) {
    int dx = wallTile->getRow() - current->getRow();
    int dy = wallTile->getCol() - current->getCol();

    int stepX = (dx != 0) ? dx / std::abs(dx) : 0;
    int stepY = (dy != 0) ? dy / std::abs(dy) : 0;

    return current->getNeighbor(-2 * stepX, -2 * stepY);
}

void Pathfinding::attackWall(std::shared_ptr<Tile> wallTile) {
    // Placeholder for attacking the wall
    // Implement the actual attack logic here
    wallTile->takeDamage(10); // Example damage value
}