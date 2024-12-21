// GameState.hpp
#ifndef GAMESTATE_HPP
#define GAMESTATE_HPP

#include "Tile.hpp"
#include <vector>
#include <string>

// Structure to store individual tile states
struct TileState {
    TileType type;
    std::string texturePath;
    int buildingId;
    std::string buildingTexturePath;
};

class GameState {
public:
    // Constructs a GameState by capturing the current tile properties
    GameState(const std::vector<std::vector<std::shared_ptr<Tile>>>& tiles);

    // Copy constructor
    GameState(const GameState& other);

    // Retrieves the stored tile states
    const std::vector<std::vector<TileState>>& getTileStates() const;

private:
    std::vector<std::vector<TileState>> tileStates;
};

#endif // GAMESTATE_HPP