#include "Tile.hpp"
#include "Building.hpp" // Include Building for implementation
#include "TextureManager.hpp"
#include "IsometricUtils.hpp"
#include <iostream>

// Constructor to initialize a Tile with a specific type
Tile::Tile(int row, int col, TileType type)
    : type(type), building(nullptr), row(row), col(col) {
    updateTexture();
}

void Tile::setType(TileType type) {
    this->type = type;
    updateTexture(); // Update texture when type changes
}

TileType Tile::getType() const {
    return type; // Return the tile's current type
}

void Tile::setBuilding(std::shared_ptr<Building> buildingPtr) {
    building = buildingPtr;
    if (buildingPtr) {
        setType(TileType::Building); // Change type to Building if a building exists
    } else {
        setType(TileType::Grass); // Default to Grass if no building
    }
}

std::shared_ptr<Building> Tile::getBuilding() const {
    return building; // Return the associated building
}

void Tile::setPosition(float x, float y) {
    sprite.setPosition(x, y); // Set the position of the sprite representing the tile
}

sf::Vector2f Tile::getPosition() const {
    return sprite.getPosition(); // Get the current position of the sprite
}

void Tile::updateTexture() {
    TextureManager& tm = TextureManager::getInstance();
    std::shared_ptr<sf::Texture> texturePtr = nullptr;

    // Select texture based on tile type
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
        sprite.setTexture(*texturePtr); // Apply texture to sprite
        sprite.setScale(
            static_cast<float>(TILE_WIDTH) / texturePtr->getSize().x,
            static_cast<float>(TILE_HEIGHT) / texturePtr->getSize().y
        ); // Ensure texture is scaled correctly to fit tile dimensions
    } else {
        std::cerr << "Failed to load texture for tile type." << std::endl;
    }
}

void Tile::draw(sf::RenderWindow& window) const {
    window.draw(sprite); // Draw the tile's sprite on the window
    if (building) {
        // Draw the building associated with this tile
        building->draw(window);
    }
}

bool Tile::isWalkable() const {
    return type != TileType::Water && !building; // Walkable if not water and no building
}
