#include "GameState.hpp"
#include "Building.hpp"
#include "Tile.hpp"

// Constructor that captures the current state of all tiles
GameState::GameState(const std::vector<std::vector<std::shared_ptr<Tile>>>& tiles) {
    tileStates.resize(tiles.size());
    for (size_t row = 0; row < tiles.size(); ++row) {
        tileStates[row].resize(tiles[row].size());
        for (size_t col = 0; col < tiles[row].size(); ++col) {
            const auto& tile = tiles[row][col];
            TileState tileState;
            tileState.row = tile->getRow();
            tileState.col = tile->getCol();
            tileState.type = tile->getType();
            tileState.texturePath = tile->getTexturePath();
            tileState.grassIndex = tile->getGrassTileIndex();
            if (tile->getBuilding()) {
                tileState.buildingId = tile->getBuilding()->getId();
                tileState.buildingTexturePath = tile->getBuilding()->getTexturePath();
            } else {
                tileState.buildingId = -1;
            }
            if (tile->hasTrap()) {
                tileState.hasTrap = true;
                tileState.trapTexturePath = tile->getTrap()->getTexturePath();
            } else {
                tileState.hasTrap = false;
            }
            if (tile->getTower()) {
                tileState.hasTower = true;
                tileState.towerId = tile->getTower()->getId();
                tileState.towerTexturePath = tile->getTower()->getTexturePath();
            } else {
                tileState.hasTower = false;
            }
            if (tile->isWall()) {
                tileState.hasWall = true;
            } else {
                tileState.hasWall = false;
            }
            tileStates[row][col] = tileState;
        }
    }
}

// Copy constructor
GameState::GameState(const GameState& other) : tileStates(other.tileStates) {}

// Getter for tile states
const std::vector<std::vector<TileState>>& GameState::getTileStates() const {
    return tileStates;
}