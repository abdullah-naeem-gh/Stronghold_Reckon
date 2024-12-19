#ifndef TANKSPAWN_HPP
#define TANKSPAWN_HPP

#include "Tank.hpp"
#include <vector>
#include <SFML/Graphics.hpp>
#include "Map.hpp"
#include "IsometricUtils.hpp"
#include "Pathfinding.hpp"

class TankSpawn {
public:
    TankSpawn(const Map& map); // Only Map is needed here
    void handleEvent(const sf::Event& event, Map& map);
    void draw(sf::RenderWindow& window, float deltaTime);
    static const int START_X = 400;
    static const int START_Y = 50;

    // Access tanks for drawing purposes
    const std::vector<Tank>& getTanks() const;

private:
    std::vector<Tank> tanks;
    const std::string TANK_TEXTURE_PATH = "../assets/enemies/tank/tank_left.png";
    void spawnTankOnPresetTile(Map& map);
    std::vector<TileCoordinates> presetTiles;
    Pathfinding pathFinder;

    TileCoordinates townHall = {36, 19}; // Static town hall location

};

#endif // TANKSPAWN_HPP