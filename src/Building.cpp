// Building.cpp
#include "Building.hpp"
#include "TextureManager.hpp"
#include <iostream>

Building::Building(int id, float x, float y, const std::string& texturePath)
    : id(id), texturePath(texturePath) {
    auto texture = TextureManager::getInstance().getTexture(texturePath);
    if (!texture) {
        std::cerr << "Building texture not loaded: " << texturePath << std::endl;
    } else {
        sprite.setTexture(*texture);
        // Set origin to the bottom center
        sprite.setOrigin(texture->getSize().x / 2.0f, texture->getSize().y);
        // Align building position with the tile center
        sprite.setPosition(x, y);

        // Ensure no excessive scaling is applied initially
        sprite.setScale(1.0f, 1.0f);
    }
}

// Copy constructor for deep copying
Building::Building(const Building& other)
    : id(other.id), texturePath(other.texturePath) {
    auto texture = TextureManager::getInstance().getTexture(texturePath);
    if (texture) {
        sprite.setTexture(*texture);
        sprite.setOrigin(other.sprite.getOrigin());
        sprite.setPosition(other.sprite.getPosition());
        sprite.setScale(other.sprite.getScale());
    }
}

int Building::getId() const {
    return id;
}

std::string Building::getTexturePath() const {
    return texturePath;
}

void Building::setPosition(float x, float y) {
    sprite.setPosition(x, y);
}

sf::Vector2f Building::getPosition() const {
    return sprite.getPosition();
}

void Building::draw(sf::RenderWindow& window) const {
    window.draw(sprite);
}