// GameState.cpp
#include "GameState.hpp"

GameState::GameState(const std::vector<std::vector<std::shared_ptr<Tile>>>& tiles) {
    // Deep copy of tiles
    this->tiles.reserve(tiles.size());
    for (const auto& row : tiles) {
        std::vector<std::shared_ptr<Tile>> newRow;
        newRow.reserve(row.size());
        for (const auto& tile : row) {
            if (tile) {
                newRow.emplace_back(std::make_shared<Tile>(*tile)); // Invokes Tile's copy constructor
            }
            else {
                newRow.emplace_back(nullptr);
            }
        }
        this->tiles.emplace_back(newRow);
    }
}

const std::vector<std::vector<std::shared_ptr<Tile>>>& GameState::getTiles() const {
    return tiles;
}