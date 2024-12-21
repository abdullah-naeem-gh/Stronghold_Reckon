#include "Tank.hpp"
#include "TextureManager.hpp"
#include <iostream>
#include <cmath>

Tank::Tank(float x, float y, const std::vector<std::shared_ptr<Tile>>& path)
    : path(path), currentPathIndex(0) {
    loadTextures();
    
    if (!directionTextures.empty()) {
        sprite.setTexture(*directionTextures["left"]); // Start with the left-facing texture
    }
    
    sprite.setOrigin(static_cast<float>(TANK_WIDTH) / 2.0f, static_cast<float>(TANK_HEIGHT));
    sprite.setPosition(x, y);
    sprite.setScale(static_cast<float>(TANK_WIDTH) / sprite.getLocalBounds().width,
                    static_cast<float>(TANK_HEIGHT) / sprite.getLocalBounds().height);
}

// Load texture for each direction using fixed paths
void Tank::loadTextures() {
    std::map<std::string, std::string> directionPaths = {
        {"up", "../assets/enemies/tank/tank_up.png"},
        {"down", "../assets/enemies/tank/tank_down.png"},
        {"left", "../assets/enemies/tank/tank_left.png"},
        {"right", "../assets/enemies/tank/tank_right.png"},
        {"up_right", "../assets/enemies/tank/tank_up_right.png"},
        {"up_left", "../assets/enemies/tank/tank_up_left.png"},
        {"down_right", "../assets/enemies/tank/tank_down_right.png"},
        {"down_left", "../assets/enemies/tank/tank_down_left.png"}
    };

    for (const auto& [dir, path] : directionPaths) {
        auto texture = TextureManager::getInstance().getTexture(path);
        if (texture) {
            directionTextures[dir] = texture;
        } else {
            std::cerr << "Tank texture not loaded: " << path << std::endl;
        }
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

void Tank::setDirection(float dx, float dy) {
    if (dx == 0 && dy < 0) {
        sprite.setTexture(*directionTextures["up"]);
    } else if (dx == 0 && dy > 0) {
        sprite.setTexture(*directionTextures["down"]);
    } else if (dy == 0 && dx < 0) {
        sprite.setTexture(*directionTextures["left"]);
    } else if (dy == 0 && dx > 0) {
        sprite.setTexture(*directionTextures["right"]);
    } else if (dx > 0 && dy < 0) {
        sprite.setTexture(*directionTextures["up_right"]);
    } else if (dx < 0 && dy < 0) {
        sprite.setTexture(*directionTextures["up_left"]);
    } else if (dx > 0 && dy > 0) {
        sprite.setTexture(*directionTextures["down_right"]);
    } else if (dx < 0 && dy > 0) {
        sprite.setTexture(*directionTextures["down_left"]);
    }
}

void Tank::move(float deltaTime) {
    if (!path.empty() && currentPathIndex < path.size()) {
        sf::Vector2f targetPos = path[currentPathIndex]->getPosition();
        sf::Vector2f direction = targetPos - sprite.getPosition();
        float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);

        if (distance > 1.0f) {
            direction /= distance; // Normalize direction
            setDirection(direction.x, direction.y); // Update direction quicker
            sprite.move(direction * speed * deltaTime); // Move tank
        } else {
            currentPathIndex++; // Move to the next tile in the path
        }
    }
}