// Pathfinding.cpp
#include "Pathfinding.hpp"
#include <iostream>
#include <cmath>
#include <algorithm>

// Constructor
Pathfinding::Pathfinding(const Map& mapInstance)
    : mapInstance(mapInstance) {}

// Heuristic function: Manhattan distance based on tile grid coordinates
int Pathfinding::heuristic(std::shared_ptr<Tile> a, std::shared_ptr<Tile> b) {
    // Manhattan distance based on row and column indices
    int dx = std::abs(a->getRow() - b->getRow());
    int dy = std::abs(a->getCol() - b->getCol());
    return dx + dy;
}

// Reconstructs the path from start to end
std::vector<std::shared_ptr<Tile>> Pathfinding::reconstructPath(
    std::shared_ptr<Tile> start,
    std::shared_ptr<Tile> end,
    const std::unordered_map<std::shared_ptr<Tile>, std::shared_ptr<Tile>, TilePtrHash, TilePtrEqual>& cameFrom
) {
    std::cout << "Reconstruct path entered\n";
    std::vector<std::shared_ptr<Tile>> path;
    std::shared_ptr<Tile> current = end;

    while (current != start) {
        path.push_back(current);
        std::cout << "Pushed into path: (" << current->getRow() << ", " << current->getCol() << ")\n";
        if (cameFrom.find(current) == cameFrom.end()) {
            std::cerr << "Error: CameFrom map does not contain tile (" 
                      << current->getRow() << ", " << current->getCol() << ")\n";
            break;
        }
        current = cameFrom.at(current);
    }
    path.push_back(start);
    std::reverse(path.begin(), path.end());
    return path;
}

// A* Pathfinding Algorithm
std::vector<std::shared_ptr<Tile>> Pathfinding::findPath(std::shared_ptr<Tile> start, std::shared_ptr<Tile> end) {
    std::cout << "Pathfinder entered\n";
    std::cout << "Start Position: (" << start->getRow() << ", " << start->getCol() << ")\n";
    std::cout << "End Position: (" << end->getRow() << ", " << end->getCol() << ")\n";

    std::priority_queue<Node, std::vector<Node>, std::greater<Node>> openList;
    std::unordered_map<std::shared_ptr<Tile>, std::shared_ptr<Tile>, TilePtrHash, TilePtrEqual> cameFrom;
    std::unordered_map<std::shared_ptr<Tile>, int, TilePtrHash, TilePtrEqual> gScore;

    openList.push(Node{ start, 0, heuristic(start, end), heuristic(start, end) });
    gScore[start] = 0;

    while (!openList.empty()) {
        Node current = openList.top();
        openList.pop();

        if (current.tile == end) {
            return reconstructPath(start, end, cameFrom);
        }

        for (const auto& neighbor : mapInstance.getNeighbors(current.tile)) { // Use Map's getNeighbors
            if (!neighbor) continue; // Safety check
            int tentativeGScore = gScore[current.tile] + 1; // Each move has a cost of 1

            if (gScore.find(neighbor) == gScore.end() || tentativeGScore < gScore[neighbor]) {
                cameFrom[neighbor] = current.tile;
                gScore[neighbor] = tentativeGScore;
                int h = heuristic(neighbor, end);
                openList.push(Node{ neighbor, tentativeGScore, h, tentativeGScore + h });
            }
        }
    }

    // If no path is found, return an empty path
    std::cout << "No path found to the tile at: (" 
              << end->getRow() << ", " << end->getCol() << ")\n";
              
    return std::vector<std::shared_ptr<Tile>>{};
}