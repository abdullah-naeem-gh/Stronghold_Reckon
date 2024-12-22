#ifndef BULLETMANAGER_HPP
#define BULLETMANAGER_HPP

#include "Bullet.hpp"
#include "QuadTree.hpp"
#include <SFML/Graphics.hpp>
#include <vector>

class BulletManager {
public:
    BulletManager(sf::FloatRect bounds);
    void fireBullet(sf::Vector2f startPos, sf::Vector2f targetPos, float speed);
    void update(float deltaTime);
    void render(sf::RenderWindow& window) const;

private:
    std::vector<Bullet> bullets;
    QuadTree quadTree;  // Quad-Tree for spatial management
};

#endif // BULLETMANAGER_HPP