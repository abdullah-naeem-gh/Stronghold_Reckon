#include "TankSpawn.hpp"
#include "IsometricUtils.hpp"
#include <cstdlib>
#include <ctime>

// Constructor initializes preset tiles and seeds the random number generator
TankSpawn::TankSpawn(const Map& map)
    : presetTiles({ {0, 29} /* Add other preset tiles here if needed */}),
      pathFinder(map) {
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
}

// Handles events related to tank spawning
void TankSpawn::handleEvent(const sf::Event& event, Map& map) {
    // Handle key press for spawning tanks (e.g., pressing 'P' to spawn a tank)
    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::P) {
        spawnTankOnPresetTile(map);
    }
}

// Spawns a tank on a randomly selected preset tile
void TankSpawn::spawnTankOnPresetTile(Map& map) {
       if (!presetTiles.empty()) {
           // Select a random preset tile
           TileCoordinates spawnLocation = presetTiles[rand() % presetTiles.size()];
           auto tile = map.getTile(spawnLocation.row, spawnLocation.col);
           if (!tile || tile->getBuilding()) {
               std::cerr << "Invalid or occupied tile at (" << spawnLocation.row << ", " << spawnLocation.col << ").\n";
               return;
           }
           sf::Vector2f isoPos = IsometricUtils::tileToScreen(spawnLocation.row, spawnLocation.col);
           sf::Vector2f tankPosition = sf::Vector2f(isoPos.x, isoPos.y + Tile::TILE_HEIGHT);
           // Implement pathfinding from spawn location to town hall
           std::vector<std::shared_ptr<Tile>> path = pathFinder.findPath(
               map.getTile(spawnLocation.row, spawnLocation.col),
               map.getTile(townHall.row, townHall.col)
           );
           if (path.empty()) {
               std::cerr << "No available path from (" << spawnLocation.row << ", " << spawnLocation.col
                         << ") to (" << townHall.row << ", " << townHall.col << ").\n";
               return;
           }
           tanks.emplace_back(Tank(tankPosition.x, tankPosition.y, path));
           std::cout << "Tank placed at tile: (" << spawnLocation.row << ", " << spawnLocation.col << ").\n";
           std::cout << "Path to be followed by tank:\n";
           for (const auto& step : path) {
               if (step) {
                   std::cout << "{ " << step->getRow() << ", " << step->getCol() << " }\n";
               }
           }
       }
       else {
           std::cerr << "No preset tiles available for spawning tanks.\n";
       }
   }

// Draws all active tanks
void TankSpawn::draw(sf::RenderWindow& window, float deltaTime) {
    for (auto& tank : tanks) {
        tank.move(deltaTime); // Update tank position based on delta time
        tank.draw(window); // Render the tank on the window
    }
}

// Returns a const reference to the tanks vector
const std::vector<Tank>& TankSpawn::getTanks() const {
    return tanks;
}