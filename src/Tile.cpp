// Tile.cpp
#include "Tile.hpp"
#include "Building.hpp"
#include "TextureManager.hpp"
#include "IsometricUtils.hpp"
#include <iostream>

Tile::Tile(int row, int col, TileType type)
    : type(type), building(nullptr), row(row), col(col), blockStatus(false) {
    updateTexture();
}

Tile::Tile(const Tile& other)
    : type(other.type), building(nullptr), row(other.row), col(other.col), blockStatus(other.blockStatus) {
    sprite = other.sprite; // sf::Sprite supports copy assignment
    if (other.building) {
        building = std::make_shared<Building>(*other.building); // Deep copy
        setType(TileType::Building);
    }
    else {
        setType(TileType::Grass);
    }
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
        blockStatus = true;
    }
    else {
        setType(TileType::Grass);
        blockStatus = false;
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
    }
    else {
        std::cerr << "Failed to load texture for tile type." << std::endl;
    }
}

void Tile::draw(sf::RenderWindow& window) const {
    window.draw(sprite);
    if (building) {
        building->draw(window);
    }

    // Uncomment the following lines for debugging tile boundaries
    /*
    sf::RectangleShape outline(sf::Vector2f(TILE_WIDTH, TILE_HEIGHT));
    outline.setPosition(sprite.getPosition());
    outline.setFillColor(sf::Color::Transparent);
    outline.setOutlineColor(sf::Color::Red);
    outline.setOutlineThickness(1.0f);
    window.draw(outline);
    */
}

bool Tile::isBlocked() const {
    return this->blockStatus;
}

int Tile::getRow() const {
    return row;
}

int Tile::getCol() const {
    return col;
}