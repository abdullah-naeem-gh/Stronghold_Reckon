// GameState.cpp
#include "GameState.hpp"
#include "Building.hpp"

GameState::GameState(const std::vector<std::vector<std::shared_ptr<Tile>>>& tiles) {
    tileStates.resize(tiles.size(), std::vector<TileState>(tiles[0].size()));
    for (size_t row = 0; row < tiles.size(); ++row) {
        for (size_t col = 0; col < tiles[row].size(); ++col) {
            auto tile = tiles[row][col];
            if (tile) {
                tileStates[row][col].type = tile->getType();
                tileStates[row][col].texturePath = tile->getTexturePath();
                if (tile->getBuilding()) {
                    tileStates[row][col].buildingId = tile->getBuilding()->getId();
                    tileStates[row][col].buildingTexturePath = tile->getBuilding()->getTexturePath();
                } else {
                    tileStates[row][col].buildingId = -1;
                    tileStates[row][col].buildingTexturePath = "-";
                }
            }
        }
    }
}

GameState::GameState(const GameState& other) : tileStates(other.tileStates) {}

const std::vector<std::vector<TileState>>& GameState::getTileStates() const {
    return tileStates;
}