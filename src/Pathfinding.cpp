#include "Pathfinding.hpp"
#include <queue>
#include <unordered_map>
#include <iostream>
#include <cmath>

Pathfinding::Pathfinding(const Map& mapInstance)
    : mapInstance(mapInstance) {}

std::vector<std::shared_ptr<Tile>> Pathfinding::findPath(std::shared_ptr<Tile> start, std::shared_ptr<Tile> end) {
    std::cout << "PATHFINDER ENTERED\n";
    std::cout << "Start Position: (" << start->row << ", " << start->col << ")\n";
    std::cout << "End Position: (" << end->row << ", " << end->col << ")\n";


    std::priority_queue<Node, std::vector<Node>, std::greater<Node>> openList;
    std::unordered_map<std::shared_ptr<Tile>, std::shared_ptr<Tile>> cameFrom;
    std::unordered_map<std::shared_ptr<Tile>, int> gScore;

    openList.push(Node{start, 0, heuristic(start, end), heuristic(start, end)});
    gScore[start] = 0;

    while (!openList.empty()) {
        Node current = openList.top();
        openList.pop();

        if (current.tile == end) {
            return reconstructPath(start, end, cameFrom);
        }

        for (const auto& neighbor : mapInstance.getNeighbors(current.tile)) { // Use Map's getNeighbors
            int tentativeGScore = gScore[current.tile] + 1; // Each move has a cost of 1

            if (gScore.find(neighbor) == gScore.end() || tentativeGScore < gScore[neighbor]) {
                cameFrom[neighbor] = current.tile;
                gScore[neighbor] = tentativeGScore;
                openList.push(Node{neighbor, tentativeGScore, heuristic(neighbor, end), tentativeGScore + heuristic(neighbor, end)});
            }
        }
    }

    // If no path is found, return an empty path
    std::cout << "no path found to the tile at: (" << end->getRow() << ", " << end->getCol() << ")\n";
    return std::vector<std::shared_ptr<Tile>>{};
}

std::vector<std::shared_ptr<Tile>> Pathfinding::reconstructPath(
    std::shared_ptr<Tile> start, 
    std::shared_ptr<Tile> end, 
    const std::unordered_map<std::shared_ptr<Tile>, std::shared_ptr<Tile>>& cameFrom
) {
    std::cout << "Reconstruct path entered ENTERED\n";

    std::vector<std::shared_ptr<Tile>> path;
    std::shared_ptr<Tile> current = end;

    while (current != start) {
        path.push_back(current);
        std::cout << "pushed into path, " << current->getRow() << ", " << current->getCol() << "\n";
        current = cameFrom.at(current); // Use .at() for safer access
    }

    path.push_back(start);
    std::reverse(path.begin(), path.end());

    
    return path;
}

int Pathfinding::heuristic(std::shared_ptr<Tile> a, std::shared_ptr<Tile> b) {
    // Manhattan distance (assuming a grid layout)
    int dx = std::abs(a->getPosition().x - b->getPosition().x);
    int dy = std::abs(a->getPosition().y - b->getPosition().y);
    return dx + dy;
}
