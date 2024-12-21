// Tile.cpp
#include "Tile.hpp"
#include "Building.hpp"
#include "TextureManager.hpp"
#include "IsometricUtils.hpp"
#include <iostream>
#include <random>

// Constructor
Tile::Tile(int row, int col, TileType type)
    : type(type), row(row), col(col), blockStatus(false), building(nullptr), texturePath("") {
    updateTexture();
}

// Copy Constructor
Tile::Tile(const Tile& other)
    : type(other.type), row(other.row), col(other.col), blockStatus(other.blockStatus),
      texturePath(other.texturePath), building(nullptr) {
    // Load the same texture
    auto texturePtr = TextureManager::getInstance().getTexture(texturePath);
    if (texturePtr) {
        sprite.setTexture(*texturePtr);
        sprite.setScale(
            static_cast<float>(TILE_WIDTH) / static_cast<float>(texturePtr->getSize().x),
            static_cast<float>(TILE_HEIGHT) / static_cast<float>(texturePtr->getSize().y)
        );
    }
    sprite.setOrigin(other.sprite.getOrigin());
    sprite.setPosition(other.sprite.getPosition());
    sprite.setScale(other.sprite.getScale());
    
    // Copy building if exists
    if (other.building) {
        building = std::make_shared<Building>(*other.building); // Deep copy
    }
}

// Setter for texturePath
void Tile::setTexturePath(const std::string& path) {
    texturePath = path;
    loadTexture();
}

// Getter for texturePath
std::string Tile::getTexturePath() const {
    return texturePath;
}

// Helper method to load texture based on type and texturePath
void Tile::loadTexture() {
    TextureManager& tm = TextureManager::getInstance();
    std::shared_ptr<sf::Texture> texturePtr;

    switch (type) {
    case TileType::Grass:
        if (!texturePath.empty()) {
            texturePtr = tm.getTexture(texturePath);
        } else {
            // Randomly choose grass texture if texturePath is not set
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_int_distribution<> dis(1, 4);
            int grassType = dis(gen);
            texturePath = "../assets/tiles/grass" + std::to_string(grassType) + ".png";
            texturePtr = tm.getTexture(texturePath);
        }
        break;
    case TileType::Water:
        texturePath = "../assets/tiles/water.png";
        texturePtr = tm.getTexture(texturePath);
        break;
    case TileType::Road:
        texturePath = "../assets/tiles/road.png";
        texturePtr = tm.getTexture(texturePath);
        break;
    case TileType::Wall:
        texturePath = "../assets/walls/brick_wall.png";
        texturePtr = tm.getTexture(texturePath);
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
        std::cerr << "Failed to load texture for tile at (" << row << ", " << col << ")\n";
    }
}

// Set Type without altering building presence
void Tile::setType(TileType newType) {
    type = newType;
    loadTexture();
    // Update blockStatus based on type and building presence
    if (building != nullptr) {
        blockStatus = true;
    }
    else {
        // Define which tile types are blocked
        switch (type) {
            case TileType::Water:
            case TileType::Wall:
                blockStatus = true;
                break;
            default:
                blockStatus = false;
                break;
        }
    }
}

TileType Tile::getType() const {
    return type;
}

void Tile::setBuilding(std::shared_ptr<Building> buildingPtr) {
    building = buildingPtr;
    if (buildingPtr) {
        // When a building is present, block the tile
        blockStatus = true;
    }
    else {
        // Re-evaluate blockStatus based on tile type
        switch (type) {
            case TileType::Water:
            case TileType::Wall:
                blockStatus = true;
                break;
            default:
                blockStatus = false;
                break;
        }
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
    loadTexture();
}

int Tile::getRow() const {
    return row;
}

int Tile::getCol() const {
    return col;
}

void Tile::addEdge(std::shared_ptr<Tile> neighbor) {
    neighbors.push_back(neighbor);
}

const std::vector<std::shared_ptr<Tile>>& Tile::getNeighbors() const {
    return neighbors;
}

std::shared_ptr<Tile> Tile::getNeighbor(int dx, int dy) const {
    for (const auto& neighbor : neighbors) {
        if (neighbor->getRow() == row + dx && neighbor->getCol() == col + dy) {
            return neighbor;
        }
    }
    return nullptr;
}

bool Tile::isBlocked() const {
    return blockStatus;
}

bool Tile::isWall() const {
    return type == TileType::Wall;
}

void Tile::draw(sf::RenderWindow& window) const {
    window.draw(sprite);
    if (building) {
        building->draw(window);
    }
}

void Tile::takeDamage(int damage) {
    // Implement tile damage logic here if applicable
    std::cout << "Tile at (" << row << ", " << col << ") took " << damage << " damage.\n";
}