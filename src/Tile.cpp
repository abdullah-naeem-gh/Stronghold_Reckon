// Tile.cpp
#include "Tile.hpp"
#include "Building.hpp"
#include "TextureManager.hpp"
#include "IsometricUtils.hpp"
#include <iostream>
#include <random>

Tile::Tile(int row, int col, TileType type)
    :   type(type), row(row), col(col), blockStatus(false), building(nullptr) {
    updateTexture();
}

Tile::Tile(const Tile& other)
    : type(other.type), row(other.row), col(other.col), blockStatus(other.blockStatus), building(nullptr) {
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
        {
            // Randomly choose one of the grass textures
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_int_distribution<> dis(1, 4);
            int grassType = dis(gen);
            
            // Construct the path based on the selected grass type
            std::string texturePath = "../assets/tiles/grass" + std::to_string(grassType) + ".png";
            texturePtr = tm.getTexture(texturePath);
        }
        break;

    case TileType::Water:
        texturePtr = tm.getTexture("../assets/tiles/water.png");
        break;

    case TileType::Road:
        texturePtr = tm.getTexture("../assets/tiles/road.png");
        break;

    case TileType::Building:
        texturePtr = tm.getTexture("../assets/tiles/default.png");
        break;

    // Add more cases if there are additional types like Wall
    }

    if (texturePtr) {
        sprite.setTexture(*texturePtr);
        // Ensure the texture fills the tile
        sprite.setScale(
            static_cast<float>(TILE_WIDTH) / static_cast<float>(texturePtr->getSize().x),
            static_cast<float>(TILE_HEIGHT) / static_cast<float>(texturePtr->getSize().y)
        );
    }
}

void Tile::draw(sf::RenderWindow& window) const {
    window.draw(sprite);
    if (building) {
        building->draw(window);
    }
}

bool Tile::isBlocked() const {
    return blockStatus;
}

bool Tile::isWall() const {
    // std::cout << "Tile type: " << this->getType() << "\n";
    return type == TileType::Wall; // Assuming TileType::Wall exists
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

void Tile::takeDamage(int damage) {
    // Implement logic to handle taking damage
    // This is a placeholder implementation
    std::cout << "Tile at (" << row << ", " << col << ") took " << damage << " damage." << std::endl;
}
