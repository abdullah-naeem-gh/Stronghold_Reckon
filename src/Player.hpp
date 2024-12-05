#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>

class Tile;

class Player {
public:
    Player(float startX, float startY);
    void draw(sf::RenderWindow& window) const;
    void setPath(const std::vector<std::shared_ptr<Tile>>& newPath);
    void updateMovement(float deltaTime);

private:
    sf::RectangleShape sprite;
    static const int PLAYER_SIZE = 32;
    float movementSpeed = 100.0f; // Movement speed per second

    std::vector<std::shared_ptr<Tile>> path;
    size_t currentPathIndex = 0;
};

#endif // PLAYER_HPP