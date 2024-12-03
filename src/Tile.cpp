// Tile.cpp
#include "Tile.hpp"
#include "Building.hpp" // Include Building for implementation
#include "TextureManager.hpp"
#include "IsometricUtils.hpp"
#include <iostream>

Tile::Tile(int row, int col, TileType type)
    : type(type), building(nullptr) { // Removed initialization of row and col
    updateTexture();
}

void Tile::setType(TileType type) {
    this->type = type;
    updateTexture();
}

TileType Tile::getType() const {
    return type;
}

void Tile::setBuilding(std::shared_ptr<Building> buildingPtr) {
    building = buildingPtr;
    if (buildingPtr) {
        setType(TileType::Building);
    }
    else {
        setType(TileType::Grass);
    }
}

std::shared_ptr<Building> Tile::getBuilding() const {
    return building;
}

void Tile::setPosition(float x, float y) {
    sprite.setPosition(x, y);
}

sf::Vector2f Tile::getPosition() const {
    return sprite.getPosition();
}

void Tile::updateTexture() {
    TextureManager& tm = TextureManager::getInstance();
    std::shared_ptr<sf::Texture> texturePtr;

    switch (type) {
    case TileType::Grass:
        texturePtr = tm.getTexture("../assets/tiles/grass.png");
        break;
    case TileType::Water:
        texturePtr = tm.getTexture("../assets/tiles/water.png");
        break;
    case TileType::Road:
        texturePtr = tm.getTexture("../assets/tiles/road.png");
        break;
    case TileType::Building:
        texturePtr = tm.getTexture("../assets/tiles/building_tile.png");
        break;
    }

    if (texturePtr) {
        sprite.setTexture(*texturePtr);
        // Ensure the texture fills the tile
        sprite.setScale(
            static_cast<float>(TILE_WIDTH) / static_cast<float>(texturePtr->getSize().x),
            static_cast<float>(TILE_HEIGHT) / static_cast<float>(texturePtr->getSize().y)
        );
    } else {
        std::cerr << "Failed to load texture for tile type." << std::endl;
    }
}

void Tile::draw(sf::RenderWindow& window) const {
    window.draw(sprite);
    if (building) {
        // std::cout << "Drawing Building at position (" << sprite.getPosition().x << ", " << sprite.getPosition().y << ").\n";
        building->draw(window);
    }
}