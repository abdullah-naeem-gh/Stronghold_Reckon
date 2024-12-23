#ifndef PATHFINDING_HPP
#define PATHFINDING_HPP

#include <memory>
#include <vector>

class Tile;
class Map;

class Pathfinding {
public:
    Pathfinding(const Map& map);

    std::vector<std::shared_ptr<Tile>> findPath(std::shared_ptr<Tile> start, std::shared_ptr<Tile> end);
    std::shared_ptr<Tile> getNextTileInStraightPath(std::shared_ptr<Tile> current, std::shared_ptr<Tile> end);
    std::shared_ptr<Tile> getTileTwoStepsBehind(std::shared_ptr<Tile> current, std::shared_ptr<Tile> wallTile);
    
private:
    const Map& map;

    void attackWall(std::shared_ptr<Tile> wallTile);
};

#endif // PATHFINDING_HPP