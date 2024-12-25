#ifndef GAMESTATE_HPP
#define GAMESTATE_HPP

#include "Tile.hpp"
#include <vector>
#include <string>

extern int tanksAtTownHall;
extern int skeletonsAtTownHall;

// Structure to store individual tile states
struct TileState {
    // TileType type;
    // std::string texturePath;
    // int buildingId;
    // std::string buildingTexturePath;
    // int grassIndex; // Added to capture grass tile index

    int row;
    int col;
    TileType type;
    std::string texturePath;
    int grassIndex;
    
    int buildingId;
    std::string buildingTexturePath;
    bool hasTrap;

    std::string trapTexturePath;
    bool hasTower;

    int towerId;
    std::string towerTexturePath;

    bool hasWall;
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