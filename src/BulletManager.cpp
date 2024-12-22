#include "BulletManager.hpp"

BulletManager::BulletManager(sf::FloatRect bounds) : quadTree(0, bounds) {}

void BulletManager::fireBullet(sf::Vector2f startPos, sf::Vector2f targetPos, float speed) {
    bullets.emplace_back(startPos, targetPos, speed); // Use emplace_back for in-place construction
    quadTree.insert(bullets.back());
}

void BulletManager::update(float deltaTime) {
    quadTree.clear(); // Clear and rebuild the Quad-Tree every update
    for (auto& bullet : bullets) {
        bullet.update(deltaTime);
        if (bullet.isActive()) {
            quadTree.insert(bullet);
        }
    }

    // Remove inactive bullets
    bullets.erase(std::remove_if(bullets.begin(), bullets.end(),
        [](const Bullet& b) { return !b.isActive(); }), bullets.end());
}

void BulletManager::render(sf::RenderWindow& window) const {
    for (const auto& bullet : bullets) {
        bullet.render(window);
    }
}