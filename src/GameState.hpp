// GameState.hpp
#ifndef GAMESTATE_HPP
#define GAMESTATE_HPP

#include "Tile.hpp"
#include <vector>

class GameState {
public:
    // Constructs a GameState by deep copying the provided tiles
    GameState(const std::vector<std::vector<std::shared_ptr<Tile>>>& tiles);
    
    // Retrieves the copied tiles
    const std::vector<std::vector<std::shared_ptr<Tile>>>& getTiles() const;

private:
    std::vector<std::vector<std::shared_ptr<Tile>>> tiles;
};

#endif // GAMESTATE_HPP