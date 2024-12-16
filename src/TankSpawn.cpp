#include "TankSpawn.hpp"
#include "IsometricUtils.hpp"
#include <iostream>
#include <cstdlib>
#include <ctime>

TankSpawn::TankSpawn() : presetTiles({
    {21, 8}, {21, 9}, {21, 10}, {21, 11}, {21, 12}, 
    {21, 13}, {22, 15}, {21, 15}, {21, 16}, {21, 17}
}) {
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
}

void TankSpawn::handleEvent(const sf::Event& event, Map& map) {
    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::P) {
        spawnTankOnPresetTile(map);
    }
}

void TankSpawn::spawnTankOnPresetTile(Map& map) {
    if (!presetTiles.empty()) {
        TileCoordinates spawnLocation = presetTiles[rand() % presetTiles.size()];
        auto tile = map.getTile(spawnLocation.row, spawnLocation.col);
        if (!tile || tile->getBuilding()) {
            std::cerr << "Invalid or occupied tile.\n";
            return;
        }

        sf::Vector2f isoPos = IsometricUtils::tileToScreen(spawnLocation.row, spawnLocation.col, START_X, START_Y);
        // With origin at bottom center, simplify positioning
        sf::Vector2f tankPosition = sf::Vector2f(
            isoPos.x + Tile::TILE_WIDTH / 2.0f,
            isoPos.y + Tile::TILE_HEIGHT
        );

        tanks.emplace_back(Tank(tankPosition.x, tankPosition.y, TANK_TEXTURE_PATH));
        std::cout << "Tank placed at tile: (" << spawnLocation.row << ", " << spawnLocation.col << ").\n";
    }
}

void TankSpawn::draw(sf::RenderWindow& window) const {
    for (const auto& tank : tanks) {
        tank.draw(window);
    }
}


const std::vector<Tank>& TankSpawn::getTanks() const {
    return tanks;
}