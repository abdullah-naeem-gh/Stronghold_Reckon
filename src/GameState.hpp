#ifndef GAMESTATE_HPP
#define GAMESTATE_HPP

#include "Tile.hpp"
#include <vector>
#include <memory>

class GameState {
public:
    GameState(const std::vector<std::vector<std::shared_ptr<Tile>>>& tiles);
    
    const std::vector<std::vector<std::shared_ptr<Tile>>>& getTiles() const;

private:
    std::vector<std::vector<std::shared_ptr<Tile>>> tiles;
};

#endif // GAMESTATE_HPP