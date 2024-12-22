// Building.cpp
#include "Building.hpp"
#include "TextureManager.hpp"
#include <iostream>
#include "Tile.hpp"

Building::Building(int id, float x, float y, const std::string& texturePath)
    : id(id), texturePath(texturePath) {
    auto texture = TextureManager::getInstance().getTexture(texturePath);
    if (!texture) {
        std::cerr << "Building texture not loaded: " << texturePath << std::endl;
    } else {
        sprite.setTexture(*texture);

        if (texturePath == "../assets/buildings/townhall.png") {
            // Set origin to bottom-left corner
            sprite.setOrigin(0.0f, 128.0f);

            // Scale width to fit 2 tiles, keep height original
            float scaleFactorX = (2 * static_cast<float>(Tile::TILE_WIDTH)) / 128.0f;
            sprite.setScale(scaleFactorX, 1.0f);

            // Position directly based on click
            sprite.setPosition(x, y);
        } else {
            sprite.setOrigin(static_cast<float>(texture->getSize().x) / 2.0f, static_cast<float>(texture->getSize().y));
            sprite.setScale(1.0f, 1.0f);
            sprite.setPosition(x, y);
        }
    }
}

// Implement the Copy Constructor
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