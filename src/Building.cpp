// Building.cpp
#include "Building.hpp"
#include "TextureManager.hpp"
#include <iostream>
#include <cmath>
#include "Tile.hpp"

Building::Building(int id, float x, float y, const std::string& texturePath)
    : id(id), texturePath(texturePath) { // Initialize texturePath
    auto texture = TextureManager::getInstance().getTexture(texturePath);
    if (!texture) {
        std::cerr << "Building texture not loaded: " << texturePath << std::endl;
    } else {
        sprite.setTexture(*texture);
        // Set origin to the bottom center of the building sprite
        sprite.setOrigin(static_cast<float>(texture->getSize().x) / 2.0f, static_cast<float>(texture->getSize().y));
        // Position to center at the bottom of the tile
        sprite.setPosition(x + Tile::TILE_WIDTH / 2.0f, y);
        sprite.setScale(
            static_cast<float>(BUILDING_WIDTH) / static_cast<float>(texture->getSize().x),
            static_cast<float>(BUILDING_HEIGHT) / static_cast<float>(texture->getSize().y)
        );
    }
}

int Building::getId() const {
    return id;
}

std::string Building::getTexturePath() const { // Add this function
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