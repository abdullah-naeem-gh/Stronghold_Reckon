// Building.cpp
#include "Building.hpp"
#include "TextureManager.hpp"
#include <iostream>

Building::Building(int id, float x, float y, const std::string& texturePath) : id(id) {
    auto texture = TextureManager::getInstance().getTexture(texturePath);
    if (!texture) {
        std::cerr << "Building texture not loaded: " << texturePath << std::endl;
    } else {
        sprite.setTexture(*texture);
        // Position the building so that its bottom center aligns with the tile's top center
        sprite.setPosition(x - (BUILDING_WIDTH / 2.0f), y - BUILDING_HEIGHT);
        // Adjust scaling based on asset size
        sprite.setScale(
            static_cast<float>(BUILDING_WIDTH) / texture->getSize().x,
            static_cast<float>(BUILDING_HEIGHT) / texture->getSize().y
        );
    }
}

int Building::getId() const {
    return id;
}

void Building::setPosition(float x, float y) {
    sprite.setPosition(x - (BUILDING_WIDTH / 2.0f), y - BUILDING_HEIGHT);
}

sf::Vector2f Building::getPosition() const {
    return sprite.getPosition();
}

void Building::draw(sf::RenderWindow& window) const {
    window.draw(sprite);
}