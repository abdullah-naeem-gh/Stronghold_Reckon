#ifndef GAMESTATE_HPP
#define GAMESTATE_HPP

#include "Tile.hpp"
#include "Building.hpp"
#include <vector>
#include <memory>

class GameState {
public:
    GameState(const std::vector<std::vector<std::shared_ptr<Tile>>>& tiles,
              const std::vector<std::shared_ptr<Building>>& buildings);

    const std::vector<std::vector<std::shared_ptr<Tile>>>& getTiles() const;
    const std::vector<std::shared_ptr<Building>>& getBuildings() const;

private:
    std::vector<std::vector<std::shared_ptr<Tile>>> tiles;
    std::vector<std::shared_ptr<Building>> buildings;
};

#endif // GAMESTATE_HPP