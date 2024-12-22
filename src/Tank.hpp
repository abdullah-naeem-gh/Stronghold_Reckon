#ifndef TANK_HPP
#define TANK_HPP

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include <memory>
#include <map>
#include "Tile.hpp"
#include "IsometricUtils.hpp"
#include "Pathfinding.hpp"

class Tank {
public:
    Tank(float x, float y, const std::vector<std::shared_ptr<Tile>>& path);
    void setPosition(float x, float y);
    sf::Vector2f getPosition() const;
    void draw(sf::RenderWindow& window) const;
    void move(float deltaTime, Map& map);

    void attackWall(Tile* wall, float deltaTime);

    static const int TANK_WIDTH = 64;
    static const int TANK_HEIGHT = 64;

private:
    sf::Sprite sprite;
    std::vector<std::shared_ptr<Tile>> path;
    size_t currentPathIndex;
    const float speed = 100.0f; // Speed of the tank

    std::map<std::string, std::shared_ptr<sf::Texture>> directionTextures;

    void setDirection(float dx, float dy);
    void loadTextures();

    TileCoordinates townHall = {14, 14};

};

#endif // TANK_HPP