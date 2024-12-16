#ifndef TANKSPAWN_HPP
#define TANKSPAWN_HPP

#include "Tank.hpp"
#include <vector>
#include <SFML/Graphics.hpp>
#include "Map.hpp"
#include "IsometricUtils.hpp"

class TankSpawn {
public:
    TankSpawn();
    void handleEvent(const sf::Event& event, Map& map);
    void draw(sf::RenderWindow& window) const;
    static const int START_X = 400;
    static const int START_Y = 50;

    // Access tanks for drawing purposes
    const std::vector<Tank>& getTanks() const;

private:
    std::vector<Tank> tanks;
    const std::string TANK_TEXTURE_PATH = "../assets/enemies/tank/tank_left.png";
    void spawnTankOnPresetTile(Map& map);
    std::vector<TileCoordinates> presetTiles;
};

#endif // TANKSPAWN_HPP