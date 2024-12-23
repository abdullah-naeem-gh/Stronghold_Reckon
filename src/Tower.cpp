#include "Tower.hpp"
#include "TextureManager.hpp"
#include "BulletManager.hpp"
#include <cmath>
#include <iostream>

Tower::Tower(int id, sf::Vector2f position, float range, float fireRate, BulletManager& centralBulletManager, const std::string& texturePath)
    : id(id), position(position), range(range), fireRate(fireRate), timeSinceLastShot(0.0f),
      bulletManager(centralBulletManager), texturePath(texturePath) {
    // Load tower texture
    auto texture = TextureManager::getInstance().getTexture(texturePath);
    if (texture) {
        towerSprite.setTexture(*texture);
        towerSprite.setOrigin(texture->getSize().x / 2.0f, texture->getSize().y / 2.0f);
        towerSprite.setPosition(position);
    } else {
        std::cerr << "Failed to load tower texture.\n";
    }

    // std::cout << "Tower created at (" << position.x << ", " << position.y << ") with id = " << id <<"\n";
}

void Tower::update(float deltaTime, const std::vector<sf::Vector2f>& troopPositions) {
    timeSinceLastShot += deltaTime;
    for (const auto& troopPosition : troopPositions) {
        if (isWithinRange(troopPosition) && timeSinceLastShot >= (1.0f / fireRate)) {
            fireAt(troopPosition);
            timeSinceLastShot = 0.0f;
            break; // Fire at one troop per update cycle
        }
    }
}

void Tower::fireAt(sf::Vector2f target) {
    bulletManager.fireBullet(position, target, 300.0f); // Use centralized BulletManager
    std::cout << "Tower fired a bullet towards (" << target.x << ", " << target.y << ").\n";
}

bool Tower::isWithinRange(sf::Vector2f troopPosition) const {
    float dx = troopPosition.x - position.x;
    float dy = troopPosition.y - position.y;
    return (dx * dx + dy * dy) <= (range * range);
}

void Tower::render(sf::RenderWindow& window) const {
    window.draw(towerSprite);
    // Bullets are managed centrally, so no need to render them here
}


int Tower::getId() const {
    return id;
}

std::string Tower::getTexturePath() const {
    return texturePath;
}