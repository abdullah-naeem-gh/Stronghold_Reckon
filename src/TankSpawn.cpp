#include "TankSpawn.hpp"
#include "IsometricUtils.hpp"
#include <iostream>
#include <cstdlib>
#include <ctime>

TankSpawn::TankSpawn(const Map& map) : presetTiles({
    {21, 8}, {21, 9}, {21, 10}, {21, 11}, {21, 12}, 
    {21, 13}, {22, 15}, {21, 15}, {21, 16}, {21, 17}
}), pathFinder(map){
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
}

void TankSpawn::handleEvent(const sf::Event& event, Map& map) {
    // std::cout << "handling tank spawn events\n";
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

/// IMPLEMENT PATHFINDING CLASS' FUNCTION HERE
        std::vector<std::shared_ptr<Tile>> path;

        std::cout << "INSIDE TANKSPAWN\n";
        std::cout << "Start Position: (" << map.getTile(spawnLocation.row, spawnLocation.col)->row << ", " << map.getTile(spawnLocation.row, spawnLocation.col)->col << ")\n";
        // std::cout << "End Position: (" << map.getTile(this->townHall.row, this->townHall.col)->row << ", " << map.getTile(this->townHall.row, this->townHall.col)->col << ")\n";
        std::cout << "End Position: (" << this->townHall.row << ", " << this->townHall.col << ")\n";


        path = pathFinder.findPath(map.getTile(spawnLocation.row, spawnLocation.col), map.getTile(this->townHall.row, this->townHall.col));

        tanks.emplace_back(Tank(tankPosition.x, tankPosition.y, TANK_TEXTURE_PATH, path));
        std::cout << "Tank placed at tile: (" << spawnLocation.row << ", " << spawnLocation.col << ").\n";
        std::cout << "Path to be followed by tank: \n";
        // Update the loop to use size_t for the index variable
        for (size_t i = 0; i < path.size(); i++) {
            std::cout << "{ " << path[i]->getRow() << ", " << path[i]->getCol() << " }\n";
        }
    }
}

void TankSpawn::draw(sf::RenderWindow& window, float deltaTime) {
    for (auto& tank : tanks) {
        tank.move(deltaTime);
        tank.draw(window);
    }
}


const std::vector<Tank>& TankSpawn::getTanks() const {
    return tanks;
}