#include "Tank.hpp"
#include "TextureManager.hpp"
#include <iostream>
#include <cmath>

Tank::Tank(float x, float y, const std::string& texturePath, std::vector<std::shared_ptr<Tile>> path)
 : texturePath(texturePath), path(path) {
    auto texture = TextureManager::getInstance().getTexture(texturePath);
    if (!texture) {
        std::cerr << "Tank texture not loaded: " << texturePath << std::endl;
    } else {
        sprite.setTexture(*texture);
        sprite.setOrigin(static_cast<float>(TANK_WIDTH) / 2.0f, static_cast<float>(TANK_HEIGHT));
        sprite.setPosition(x, y);
        sprite.setScale(
            static_cast<float>(TANK_WIDTH) / static_cast<float>(texture->getSize().x),
            static_cast<float>(TANK_HEIGHT) / static_cast<float>(texture->getSize().y)
        );
    }
}

void Tank::setPosition(float x, float y) {
    sprite.setPosition(x, y);
}

sf::Vector2f Tank::getPosition() const {
    return sprite.getPosition();
}

void Tank::draw(sf::RenderWindow& window) const {
    window.draw(sprite);
}



/// IMPLEMENT THE MOTION OF TANK ACCORDING TO THE PATH TO BE FOLLOWED


void Tank::move(float deltaTime) {
    if (!path.empty() && currentPathIndex < path.size()) {        
        sf::Vector2f targetPos = path[currentPathIndex]->getPosition();
        // std::cout << "Moving tank to: { " << targetPos.x << ", " << targetPos.y << " }\n";
        sf::Vector2f direction = targetPos - sprite.getPosition();
        float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);

        if (distance > 1.0f) {
            // Normalize direction and move tank
            direction /= distance;
            sprite.move(direction * speed * deltaTime);
        } else {
            currentPathIndex++; // Move to the next tile in the path
        }
    }
}

