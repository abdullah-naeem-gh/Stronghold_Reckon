#include "Tank.hpp"
#include "TextureManager.hpp"
#include <iostream>

Tank::Tank(float x, float y, const std::string& texturePath)
 : texturePath(texturePath) {
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