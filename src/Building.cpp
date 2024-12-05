// Building.cpp
#include "Building.hpp"
#include "TextureManager.hpp"
#include <iostream>
#include <cmath>
#include "Tile.hpp"

Building::Building(int id, float x, float y, const std::string& texturePath)
    : id(id), texturePath(texturePath) {
    auto texture = TextureManager::getInstance().getTexture(texturePath);
    if (!texture) {
        std::cerr << "Building texture not loaded: " << texturePath << std::endl;
    } else {
        sprite.setTexture(*texture);
        sprite.setOrigin(BUILDING_WIDTH / 2.0f, BUILDING_HEIGHT);  // Set origin to the bottom center
        sprite.setPosition(x + BUILDING_WIDTH / 2.0f, y);
        sprite.setScale(
            static_cast<float>(BUILDING_WIDTH) / texture->getSize().x,
            static_cast<float>(BUILDING_HEIGHT) / texture->getSize().y
        );
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

void Building::setTexture(const sf::Texture& texture) {
    sprite.setTexture(texture);
    sprite.setScale(
        static_cast<float>(BUILDING_WIDTH) / texture.getSize().x,
        static_cast<float>(BUILDING_HEIGHT) / texture.getSize().y
    );
}