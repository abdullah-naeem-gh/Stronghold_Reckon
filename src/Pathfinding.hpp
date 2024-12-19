#ifndef PATHFINDING_HPP
#define PATHFINDING_HPP

#include "Tile.hpp"
#include "Map.hpp"
#include <vector>
#include <queue>
#include <unordered_map>
#include <memory>

class Pathfinding {
public:
    explicit Pathfinding(const Map& mapInstance);
    std::vector<std::shared_ptr<Tile>> findPath(std::shared_ptr<Tile> start, std::shared_ptr<Tile> end);

private:
    const Map& mapInstance; // Reference to the Map instance for accessing tiles and neighbors

    // Reconstructs the path from the start to the end using the cameFrom map
    std::vector<std::shared_ptr<Tile>> reconstructPath(
        std::shared_ptr<Tile> start, 
        std::shared_ptr<Tile> end, 
        const std::unordered_map<std::shared_ptr<Tile>, std::shared_ptr<Tile>>& cameFrom
    );

    // Heuristic: Manhattan distance for grid-based pathfinding
    int heuristic(std::shared_ptr<Tile> a, std::shared_ptr<Tile> b);

    // A* algorithm helper struct for priority queue
    struct Node {
        std::shared_ptr<Tile> tile; // Current tile
        int gCost;                  // Cost from start to current tile
        int hCost;                  // Heuristic cost (current tile to end)
        int fCost;                  // Total cost (gCost + hCost)
        bool operator>(const Node& other) const {
            return fCost > other.fCost; // Comparison for priority queue
        }
    };
};

#endif
