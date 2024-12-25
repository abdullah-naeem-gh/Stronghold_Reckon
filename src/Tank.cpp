// Tank.cpp
#include "Tank.hpp"
#include "TextureManager.hpp"
#include "Map.hpp"
#include <iostream>
#include <cmath>
#include <thread>
#include <chrono>

Tank::Tank(float x, float y, const Map& map, const Tile& townHall)
    : map(map), townHall(townHall), pathFinder(map), currentPathIndex(0), currentState(State::Moving), speed(100.0f), health(maxHealth), explosionTime(0.0f), currentExplosionFrame(0), explosionPlaying(false) {
    loadTextures();
    // std::cout << "Tank constructor called at (" << x << ", " << y << ").\n";
    if (!directionTextures.empty()) {
        sprite.setTexture(*directionTextures["left"]); // Start with the left-facing texture
    }
    sprite.setOrigin(static_cast<float>(TANK_WIDTH) / 2.0f, static_cast<float>(TANK_HEIGHT));
    // sprite.setOrigin(static_cast<float>(TANK_WIDTH) / 2.0f, static_cast<float>(TANK_HEIGHT) / 2.0f);

    sprite.setPosition(x, y);
    sprite.setScale(
        static_cast<float>(TANK_WIDTH) / sprite.getLocalBounds().width,
        static_cast<float>(TANK_HEIGHT) / sprite.getLocalBounds().height
    );

    int row = IsometricUtils::screenToTile(x, y, map.getRows(), map.getCols()).row;
    int col = IsometricUtils::screenToTile(x, y, map.getRows(), map.getCols()).col;
    if (row >= map.getRows()) {
        row--;
    }
    if (col >= map.getCols()) {
        col--;
    }
    std::cout << "Finding path from (" << row << ", " << col << ") to town hall at (" << townHall.getRow() << ", " << townHall.getCol() << ").\n";
    path = pathFinder.findPath(map.getTile(row, col), map.getTile(townHall.getRow(), townHall.getCol()), 2);
    // for (const auto& tile : path) {
    //     std::cout << "(" << tile->getRow() << ", " << tile->getCol() << ") -> ";
    // }

    // Load explosion textures
    for (int i = 1; i <= 10; ++i) {
        std::string path = "../assets/explosions/Explosion_1/Explosion_" + std::to_string(i) + ".png";
        auto texture = TextureManager::getInstance().getTexture(path);
        if (texture) {
            explosionTextures.push_back(texture);
            // std::cout << "Explosion texture loaded: " << path << std::endl;
        } else {
            std::cerr << "Explosion texture not loaded: " << path << std::endl;
        }
    }
    if (!explosionTextures.empty()) {
        explosionSprite.setTexture(*explosionTextures[0]);
        // explosionSprite.setOrigin(explosionSprite.getLocalBounds().width / 2.0f, explosionSprite.getLocalBounds().height / 2.0f);
        explosionSprite.setOrigin(static_cast<float>(64) / 1.0f, static_cast<float>(64) / 1.0f);

    }
}

void Tank::update(float deltaTime) {
    if (currentState == State::Destroyed) {
        // std::cout << "Tank is destroyed.\n";
        if (explosionPlaying) {
            playExplosionAnimation(deltaTime);
        }
        // delete this;
        return;
    }

    switch (currentState) {
        case State::Moving:
            move(deltaTime);
            break;
        case State::AttackingWall:
            attackWall(deltaTime);
            break;
        case State::RecalculatingPath:
            recalculatePath();
            break;
        case State::Resting:
            rest();
            break; 
    }
}

void Tank::move(float deltaTime) {
    if ((path[currentPathIndex]->getRow() == townHall.getRow() + 1 || path[currentPathIndex]->getRow() == townHall.getRow() - 1 || path[currentPathIndex]->getRow() == townHall.getRow()) && (path[currentPathIndex]->getCol() == townHall.getCol() + 1 || path[currentPathIndex]->getCol() == townHall.getCol() - 1 || path[currentPathIndex]->getCol() == townHall.getCol())) {
        std::cout << "Tank reached town hall. Mission accomplished!\n";
        path.clear();
        // currentPathIndex = 0;
        currentState = State::Resting;
        tanksAtTownHall++;
    }
    if (currentState != State::Resting) {
        if (currentPathIndex < path.size()) {
            std::shared_ptr<Tile> currentTile = path[currentPathIndex];
            sf::Vector2f targetPos = currentTile->getPosition();
            sf::Vector2f direction = targetPos - sprite.getPosition();
            float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);

            if (distance > 1.0f) {
                direction /= distance;
                setDirection(direction.x, direction.y);
                sprite.move(direction * speed * deltaTime);
            } else {
                currentPathIndex++;
                checkForTrap(currentTile);
            }

            if (currentPathIndex >= path.size()) {
                // if (!((path[currentPathIndex]->getRow() == townHall.getRow() + 1 || path[currentPathIndex]->getRow() == townHall.getRow() - 1 || path[currentPathIndex]->getRow() == townHall.getRow()) && (path[currentPathIndex]->getCol() == townHall.getCol() + 1 || path[currentPathIndex]->getCol() == townHall.getCol() - 1 || path[currentPathIndex]->getCol() == townHall.getCol())))
                {
                    wallTile = map.findNearestWall(currentTile->getRow(), currentTile->getCol());
                    // wallTile = path[currentPathIndex + 1];
                    currentState = State::AttackingWall;
                    std::cout << "Tank reached wall at (" << wallTile->getRow() << ", " << wallTile->getCol() << ").\n";
                    if (wallTile->getRow() == townHall.getRow() && wallTile->getCol() == townHall.getCol()) {
                        // std::cout << "Tank reached town hall. Mission accomplished!\n";
                        path.clear();
                        // currentPathIndex = 0;
                        currentState = State::Resting;
                        tanksAtTownHall++;
                    }
                }
            }
        } 
        else {
            currentState = State::RecalculatingPath;
        }
    }
    
}

void Tank::attackWall(float deltaTime) {
    if (wallTile && wallTile->getHealth() > 0) {
        wallTile->takeDamage((0.001f * deltaTime)); // Example: Deal damage over time
        // std::cout << "Tank attacking wall at (" << wallTile->getRow() << ", " << wallTile->getCol() << "). Wall health: " << wallTile->getHealth() << "\n";
    } else {
        wallTile->setBlockStatus(false); // Unblock the wall after destroying it
        std::cout << "Wall destroyed. Recalculating path to town hall...\n";
        currentState = State::RecalculatingPath;
    }
}

void Tank::rest() {
    // Tank is resting, no actions needed
    // std::cout << "Tank is resting at the town hall.\n";
}

void Tank::recalculatePath() {
    // int x = static_cast<int>(sprite.getPosition().x);
    // int y = static_cast<int>(sprite.getPosition().y);
    // int row = IsometricUtils::screenToTile(x, y, map.getRows(), map.getCols()).row;
    // int col = IsometricUtils::screenToTile(x, y, map.getRows(), map.getCols()).col;
    int row = path[currentPathIndex - 1]->getRow();
    int col = path[currentPathIndex - 1]->getCol();
    path = pathFinder.findPath(map.getTile(row, col), map.getTile(townHall.getRow(), townHall.getCol()),2);
    currentPathIndex = 0;
    currentState = State::Moving;
}

void Tank::draw(sf::RenderWindow& window) const {
    // window.draw(sprite);
    if (currentState != State::Destroyed) {
        window.draw(sprite);
    }
    if (explosionPlaying) {
        window.draw(explosionSprite);
    }   
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
}

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

void Tank::takeDamage(int damage, float deltaTime) {
    health -= damage;
    if (health <= 0) {
        health = 0;
        std::cout << "Tank destroyed.\n";
        currentState = State::Destroyed;
        explosionPlaying = true;
        explosionTime = 0.0f;
        currentExplosionFrame = 0;
        explosionSprite.setPosition(sprite.getPosition());
    }
    std::cout << "Tank took " << damage << " damage. Health is now " << health << ".\n";
}

bool Tank::isDestroyed() const {
    return health <= 0;
}


void Tank::checkForTrap(std::shared_ptr<Tile> tile) {
    if (tile->getTrap() && tile->getTrap()->isActive()) {
        std::cout << "Tank triggered a trap at (" << tile->getRow() << ", " << tile->getCol() << ").\n";
        takeDamage(tile->getTrap()->getDamage(), 0.1f); // Example damage value
        tile->getTrap()->trigger();
    }
}


void Tank::playExplosionAnimation(float deltaTime) {

    if (explosionPlaying) {
        // std::cout << "Explosion playing.\n";
        explosionTime += deltaTime;
        if (explosionTime >= 0.1f) { // Adjust the frame duration as needed
            explosionTime = 0.0f;
            currentExplosionFrame++;
            if (currentExplosionFrame < explosionTextures.size()) {
                explosionSprite.setTexture(*explosionTextures[currentExplosionFrame]);
            } else {
                explosionPlaying = false; // Stop the animation after the last frame
            }
        }
    }
}


sf::Vector2f Tank::getPosition() const {
    return sprite.getPosition();
}

sf::Sprite Tank::getSprite() const {
    return sprite;
}