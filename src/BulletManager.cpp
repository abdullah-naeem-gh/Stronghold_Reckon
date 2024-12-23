// BulletManager.cpp
#include "BulletManager.hpp"
#include <algorithm> // For std::remove_if
#include <iostream>

void BulletManager::fireBullet(sf::Vector2f startPos, sf::Vector2f targetPos, float speed) {
    bullets.emplace_back(startPos, targetPos, speed); // Use emplace_back for in-place construction
}

void BulletManager::update(float deltaTime) {
    for (auto& bullet : bullets) {
        bullet.update(deltaTime);
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

const std::vector<Bullet>& BulletManager::getBullets() const {
    return bullets;
}

std::vector<Bullet>& BulletManager::getBullets() {
    return bullets;
}