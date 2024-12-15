#include "GameState.hpp"

// Capture both tiles and buildings in the state
GameState::GameState(const std::vector<std::vector<std::shared_ptr<Tile>>>& tiles,
                     const std::vector<std::shared_ptr<Building>>& buildings)
    : tiles(tiles), buildings(buildings) {}

const std::vector<std::vector<std::shared_ptr<Tile>>>& GameState::getTiles() const {
    return tiles;
}

const std::vector<std::shared_ptr<Building>>& GameState::getBuildings() const {
    return buildings;
}