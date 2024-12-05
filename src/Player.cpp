#include "Player.hpp"
#include "Tile.hpp"
#include <cmath>
#include <iostream>

Player::Player(float startX, float startY) {
    sprite.setSize(sf::Vector2f(PLAYER_SIZE, PLAYER_SIZE));
    sprite.setFillColor(sf::Color::Blue);
    sprite.setPosition(startX, startY);
}

void Player::setPath(const std::vector<std::shared_ptr<Tile>>& newPath) {
    path = newPath;
    currentPathIndex = 0;
}

void Player::updateMovement(float deltaTime) {
    if (currentPathIndex < path.size()) {
        auto& targetTile = path[currentPathIndex];
        sf::Vector2f targetPos = targetTile->getPosition();

        // Calculate direction and distance
        sf::Vector2f direction = targetPos - sprite.getPosition();
        float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);
        
        // Debugging: Print movement intention
        std::cout << "Moving towards: (" << targetTile->getRow() << ", " << targetTile->getCol() << ") Current position: " << sprite.getPosition().x << ", " << sprite.getPosition().y << std::endl;
        
        if (distance > 1.0f) {
            sf::Vector2f velocity = (direction / distance) * movementSpeed * deltaTime;
            sprite.move(velocity);
        } else {
            currentPathIndex++;
        }
    }
}

void Player::draw(sf::RenderWindow& window) const {
    window.draw(sprite);
}