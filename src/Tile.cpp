// Tile.cpp
#include "Tile.hpp"
#include "Building.hpp"          // Include Building for implementation
#include "TextureManager.hpp"
#include <iostream>

Tile::Tile(int row, int col, TileType type)
    : row(row), col(col), type(type), building(nullptr) {
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
    } else {
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
    switch (type) {
        case TileType::Grass:
            sprite.setTexture(*tm.getTexture("../assets/tiles/grass.png"));
            break;
        case TileType::Water:
            sprite.setTexture(*tm.getTexture("../assets/tiles/water.png"));
            break;
        case TileType::Road:
            sprite.setTexture(*tm.getTexture("../assets/tiles/road.png"));
            break;
        case TileType::Building:
            sprite.setTexture(*tm.getTexture("../assets/tiles/building_tile.png"));
            break;
    }
    // Set scaling based on tile size
    if (sprite.getTexture()) {
        sprite.setScale(
            static_cast<float>(TILE_WIDTH) / sprite.getTexture()->getSize().x,
            static_cast<float>(TILE_HEIGHT) / sprite.getTexture()->getSize().y
        );
    }
}

void Tile::draw(sf::RenderWindow& window) const {
    window.draw(sprite);
    if (building) {
        building->draw(window);
    }
}