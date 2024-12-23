// BulletManager.hpp
#ifndef BULLETMANAGER_HPP
#define BULLETMANAGER_HPP

#include "Bullet.hpp"
#include <SFML/Graphics.hpp>
#include <vector>

class BulletManager {
public:
    BulletManager() {}
    void fireBullet(sf::Vector2f startPos, sf::Vector2f targetPos, float speed);
    void update(float deltaTime);
    void render(sf::RenderWindow& window) const;
    // **Getter Methods**
    const std::vector<Bullet>& getBullets() const;
    std::vector<Bullet>& getBullets(); // Non-const getter
private:
    std::vector<Bullet> bullets;
};

#endif // BULLETMANAGER_HPP