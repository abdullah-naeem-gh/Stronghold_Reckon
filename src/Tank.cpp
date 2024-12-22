// Tank.cpp
#include "Tank.hpp"
#include "TextureManager.hpp"
#include "Map.hpp"
#include <iostream>
#include <cmath>
#include <thread>
#include <chrono>

Tank::Tank(float x, float y, const std::vector<std::shared_ptr<Tile>>& path)
    : path(path), currentPathIndex(0) {
    loadTextures();
    if (!directionTextures.empty()) {
        sprite.setTexture(*directionTextures["left"]); // Start with the left-facing texture
    }
    sprite.setOrigin(static_cast<float>(TANK_WIDTH) / 2.0f, static_cast<float>(TANK_HEIGHT));
    sprite.setPosition(x, y);
    sprite.setScale(
        static_cast<float>(TANK_WIDTH) / sprite.getLocalBounds().width,
        static_cast<float>(TANK_HEIGHT) / sprite.getLocalBounds().height
    );
}

// Load texture for each direction using fixed paths
void Tank::loadTextures() {
    std::map<std::string, std::string> directionPaths = {
        {"up", "../assets/enemies/tank/tank_up.png"},
        {"down", "../assets/enemies/tank/tank_down.png"},
        {"left", "../assets/enemies/tank/tank_left.png"},
        {"right", "../assets/enemies/tank/tank_right.png"},
        {"up_right", "../assets/enemies/tank/tank_up_right.png"},
        {"up_left", "../assets/enemies/tank/tank_up_left.png"},
        {"down_right", "../assets/enemies/tank/tank_down_right.png"},
        {"down_left", "../assets/enemies/tank/tank_down_left.png"}
    };
    for (const auto& [dir, path] : directionPaths) {
        auto texture = TextureManager::getInstance().getTexture(path);
        if (texture) {
            directionTextures[dir] = texture;
        } else {
            std::cerr << "Tank texture not loaded: " << path << std::endl;
        }
    }
}

void Tank::setPosition(float x, float y) {
    sprite.setPosition(x, y);
}

sf::Vector2f Tank::getPosition() const {
    return sprite.getPosition();
}

void Tank::draw(sf::RenderWindow& window) const {
    window.draw(sprite);
}

void Tank::setDirection(float dx, float dy) {
    if (dx == 0 && dy < 0) {
        sprite.setTexture(*directionTextures.at("up"));
    }
    else if (dx == 0 && dy > 0) {
        sprite.setTexture(*directionTextures.at("down"));
    }
    else if (dy == 0 && dx < 0) {
        sprite.setTexture(*directionTextures.at("left"));
    }
    else if (dy == 0 && dx > 0) {
        sprite.setTexture(*directionTextures.at("right"));
    }
    else if (dx > 0 && dy < 0) {
        sprite.setTexture(*directionTextures.at("up_right"));
    }
    else if (dx < 0 && dy < 0) {
        sprite.setTexture(*directionTextures.at("up_left"));
    }
    else if (dx > 0 && dy > 0) {
        sprite.setTexture(*directionTextures.at("down_right"));
    }
    else if (dx < 0 && dy > 0) {
        sprite.setTexture(*directionTextures.at("down_left"));
    }
    // Additional debug to verify texture change
    // std::cout << "Tank direction set to: " 
    //           << ((dx == 0 && dy < 0) ? "up" :
    //               (dx == 0 && dy > 0) ? "down" :
    //               (dy == 0 && dx < 0) ? "left" :
    //               (dy == 0 && dx > 0) ? "right" :
    //               (dx > 0 && dy < 0) ? "up_right" :
    //               (dx < 0 && dy < 0) ? "up_left" :
    //               (dx > 0 && dy > 0) ? "down_right" :
    //               (dx < 0 && dy > 0) ? "down_left" : "unknown") 
    //           << "\n";
}

void Tank::move(float deltaTime, Map& map) {
    if (!path.empty() && currentPathIndex < path.size()) {
        sf::Vector2f targetPos = path[currentPathIndex]->getPosition();
        sf::Vector2f direction = targetPos - sprite.getPosition();
        float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);
        if (distance > 1.0f) {
            direction /= distance; // Normalize direction
            setDirection(direction.x, direction.y); // Update direction
            sprite.move(direction * speed * deltaTime); // Move tank
        }
        else {
            currentPathIndex++; // Move to the next tile in the path
        }
    }
    // else if (){
        
    // }

    else if (currentPathIndex >= path.size() && getPosition().x != IsometricUtils::tileToScreen(townHall.row, townHall.col).x ||
             getPosition().y != IsometricUtils::tileToScreen(townHall.row, townHall.col).y) {
        
        std::cout << "Wall encountered. Attacking wall...\n";
// Identify the wall tile two tiles in front of the current tile
        Pathfinding pathFinder(map);
        TileCoordinates tankTileCoord = {path[currentPathIndex - 1]->getRow(), path[currentPathIndex - 1]->getCol()};
        std::cout << "Tank at (" << tankTileCoord.row << ", " << tankTileCoord.col << ").\n";
        std::shared_ptr<Tile> currentTile = map.getTile(tankTileCoord.row, tankTileCoord.col);
        std::shared_ptr<Tile> nextTile = pathFinder.getNextTileInStraightPath(currentTile, map.getTile(townHall.row, townHall.col));
        std::cout << "Next tile: (" << nextTile->getRow() << ", " << nextTile->getCol() << ").\n";
        // std::shared_ptr<Tile> wallTile = pathFinder.getNextTileInStraightPath(nextTile, map.getTile(townHall.row, townHall.col));

        std::shared_ptr<Tile> wallTile = map.findNearestWall(nextTile->getRow(), nextTile->getCol());
        std::cout << "Wall encountered at (" << wallTile->getRow() << ", " << wallTile->getCol() << "), health: " << wallTile->getHealth() << ".\n";


        // Attack the wall until its health falls below zero
        while (wallTile && wallTile->getHealth() > 0) {
            wallTile->takeDamage(10);
            std::cout << "Tank attacking wall at (" << wallTile->getRow() << ", " << wallTile->getCol() << "). Wall health: " << wallTile->getHealth() << "\n";
            std::this_thread::sleep_for(std::chrono::milliseconds(500)); // Slight delay in attacking
        }

        wallTile->setBlockStatus(false); // Unblock the wall after destroying it
        // Recalculate path to town hall after destroying the wall
        std::cout << "Wall destroyed. Recalculating path to town hall...\n";
        path = pathFinder.findPath(map.getTile(tankTileCoord.row, tankTileCoord.col), map.getTile(townHall.row, townHall.col));
        currentPathIndex = 0;
    }
}