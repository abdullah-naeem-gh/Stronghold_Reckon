#include "Skeleton.hpp"
#include "TextureManager.hpp"
#include <cmath>
#include <iostream>
#include "Tile.hpp"

Skeleton::Skeleton(float x, float y, const std::vector<std::shared_ptr<Tile>>& path, const Map& map)
    : path(path), currentPathIndex(0), currentAnimationFrame(0), health(10), map(map), currentWall(nullptr), pathFinder(map) {
    loadTextures();
    if (!directionTextures["left"].empty()) {
        sprite.setTexture(*directionTextures["left"][0]);
    }
    sf::IntRect textureRect(96, 96, 64, 64);
    sprite.setTextureRect(textureRect);
    sprite.setOrigin(32.0f, 32.0f);
    sprite.setScale(1.0f, 1.0f);
    sprite.setPosition(x, y);

    // Load explosion textures
    for (int i = 1; i <= 8; ++i) {
        std::string pathStr = "../assets/explosions/Explosion_/Explosion_" + std::to_string(i) + ".png";
        auto texture = TextureManager::getInstance().getTexture(pathStr);
        if (texture) {
            explosionTextures.push_back(texture);
        } else {
            std::cerr << "Explosion texture not loaded: " << pathStr << std::endl;
        }
    }

    if (!explosionTextures.empty()) {
        explosionSprite.setTexture(*explosionTextures[0]);
        explosionSprite.setOrigin(explosionSprite.getLocalBounds().width / 2.0f, explosionSprite.getLocalBounds().height / 2.0f);
    }
}

void Skeleton::loadTextures() {
    std::map<std::string, std::string> directionFolders = {
        {"up", "up/"},
        {"down", "down/"},
        {"left", "left/"},
        {"right", "right/"},
        {"up_right", "up_right/"},
        {"up_left", "up_left/"},
        {"down_right", "down_right/"},
        {"down_left", "down_left/"}
    };
    std::string basePath = "../assets/enemies/skeletons/";
    for (const auto& [dir, folder] : directionFolders) {
        std::vector<std::shared_ptr<sf::Texture>> textures;
        for (int i = 1; i <= 8; ++i) {
            std::string texturePath = basePath + folder + "skeleton_" + dir + "_" + std::to_string(i) + ".png";
            auto texture = TextureManager::getInstance().getTexture(texturePath);
            if (texture) {
                textures.push_back(texture);
            } else {
                std::cerr << "Skeleton texture not loaded: " << texturePath << std::endl;
            }
        }
        directionTextures[dir] = textures;
    }
}

void Skeleton::setPosition(float x, float y) {
    sprite.setPosition(x, y);
}

sf::Vector2f Skeleton::getPosition() const {
    return sprite.getPosition();
}

void Skeleton::draw(sf::RenderWindow& window, float deltaTime) {
    // Update animation frames
    animationTime += deltaTime;
    if (animationTime >= animationFrameDuration) {
        animationTime = 0.0f;
        currentAnimationFrame = (currentAnimationFrame + 1) % 8;
        if (!path.empty() && currentPathIndex < path.size()) {
            sf::Vector2f targetPos = path[currentPathIndex]->getPosition();
            sf::Vector2f direction = targetPos - sprite.getPosition();
            setDirection(direction.x, direction.y);
        }
    }

    // Draw the sprite if not exploding and not dead
    if (!explosionPlaying && !isDead) {
        window.draw(sprite);
    }

    // Draw the explosion sprite if it's still playing
    if (explosionPlaying) {
        window.draw(explosionSprite);
    }
}

void Skeleton::setDirection(float dx, float dy) {
    std::string directionKey = "left";
    if (dx == 0 && dy < 0) directionKey = "up";
    else if (dx == 0 && dy > 0) directionKey = "down";
    else if (dy == 0 && dx < 0) directionKey = "left";
    else if (dy == 0 && dx > 0) directionKey = "right";
    else if (dx > 0 && dy < 0) directionKey = "up_right";
    else if (dx < 0 && dy < 0) directionKey = "up_left";
    else if (dx > 0 && dy > 0) directionKey = "down_right";
    else if (dx < 0 && dy > 0) directionKey = "down_left";

    if (directionTextures.find(directionKey) != directionTextures.end()) {
        sprite.setTexture(*directionTextures[directionKey][currentAnimationFrame]);
    }
}

void Skeleton::move(float deltaTime) {
    if (isDead) {
        if (explosionPlaying) {
            playExplosionAnimation(deltaTime);
        }
        return;
    }

    if (!path.empty() && currentPathIndex < path.size()) {
        auto currentTile = path[currentPathIndex];
        sf::Vector2f targetPos = currentTile->getPosition();
        sf::Vector2f direction = targetPos - sprite.getPosition();
        float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);

        // Check for walls in the path
        if (currentTile->getType() == TileType::Wall) {
            std::cout << "Skeleton encountered wall at (" 
                      << currentTile->getRow() << ", " 
                      << currentTile->getCol() << ")\n";
            currentWall = currentTile;
            damageWall();  // Call damageWall instead of explodeWall
            recalculatePath();
            return;
        }

        if (distance > 1.0f) {
            direction /= distance;
            setDirection(direction.x, direction.y);
            sprite.move(direction * speed * deltaTime);
        } else {
            checkForTrap(currentTile);
            currentPathIndex++;
        }
    }
}

void Skeleton::takeDamage(int damage) {
    health -= damage;
    if (health <= 0) {
        health = 0;
        std::cout << "Skeleton destroyed at position ("
                  << sprite.getPosition().x << ", "
                  << sprite.getPosition().y << ").\n";
        isDead = true;
        explosionPlaying = true;
        explosionTime = 0.0f;
        currentExplosionFrame = 0;
        explosionSprite.setPosition(sprite.getPosition());
    } else {
        std::cout << "Skeleton took " << damage
                  << " damage, remaining health: " << health << ".\n";
    }
}

bool Skeleton::isAlive() const {
    return health > 0;
}

sf::Sprite& Skeleton::getSprite() {
    return sprite;
}

bool Skeleton::isDestroyed() const {
    return health <= 0;
}

void Skeleton::checkForTrap(std::shared_ptr<Tile> tile) {
    if (tile->getTrap() && tile->getTrap()->isActive()) {
        std::cout << "Skeleton triggered a trap at (" 
                  << tile->getRow() << ", " 
                  << tile->getCol() << ").\n";
        takeDamage(tile->getTrap()->getDamage()); // Example damage value
        tile->getTrap()->trigger();
    }
}

void Skeleton::playExplosionAnimation(float deltaTime) {
    if (explosionPlaying) {
        explosionTime += deltaTime;
        if (explosionTime >= 0.1f) { // Adjust frame duration as needed
            explosionTime = 0.0f;
            currentExplosionFrame++;
            if (currentExplosionFrame < explosionTextures.size()) {
                explosionSprite.setTexture(*explosionTextures[currentExplosionFrame]);
                std::cout << "Displaying explosion frame: " << currentExplosionFrame << std::endl;  // Debug
            } else {
                explosionPlaying = false; // Stop animation
                currentExplosionFrame = 0; // Reset for future explosions
                // Option 1: Hide the explosion sprite by setting an empty texture rect
                // explosionSprite.setTextureRect(sf::IntRect());

                // Option 2: Move the sprite off-screen
                explosionSprite.setPosition(-1000, -1000);

                std::cout << "Explosion animation completed and sprite hidden.\n"; // Debug
            }
        }
    }
}

void Skeleton::damageWall() {
    if (currentWall) {
        const int damage = 35;
        currentWall->takeDamage(damage);  // Apply damage to the wall

        // Check if the wall's health has reached zero
        if (currentWall->getHealth() <= 0) {
            explodeWall();  // Trigger explosion
        } else {
            std::cout << "Wall at (" << currentWall->getRow() << ", " 
                      << currentWall->getCol() << ") took " << damage 
                      << " damage, remaining health: " << currentWall->getHealth() << ".\n";
        }
    }
}

void Skeleton::explodeWall() {
    if (currentWall) {
        // Change the wall to a passable tile type
        currentWall->setType(TileType::Grass);
        currentWall->setBlockStatus(false);
        currentWall->setHealth(0);
        currentWall->setBuilding(nullptr);
        currentWall->updateTexture();  // Reflect changes visually

        // Trigger explosion animation
        explosionPlaying = true;
        explosionTime = 0.0f;
        currentExplosionFrame = 0;
        explosionSprite.setPosition(currentWall->getPosition());

        std::cout << "Wall destroyed by skeleton at (" 
                  << currentWall->getRow() << ", " 
                  << currentWall->getCol() << ")\n";

        // Clear currentWall to avoid repeated actions
        currentWall = nullptr;
    }
}

void Skeleton::recalculatePath() {
    // Get current position in tile coordinates
    auto currentPos = sprite.getPosition();
    auto currentTileCoords = IsometricUtils::screenToTile(
        currentPos.x, 
        currentPos.y, 
        map.getRows(), 
        map.getCols()
    );

    // Find path to town hall
    auto currentTile = map.getTile(currentTileCoords.row, currentTileCoords.col);
    auto townHallTile = map.getTile(14, 14); // Ensure this matches your actual town hall coordinates

    if (!townHallTile) {
        std::cerr << "Town hall tile not found at (14, 14).\n";
        return;
    }

    // Recalculate path with stopBeforeWallTiles = 0 for skeletons
    path = pathFinder.findPath(currentTile, townHallTile, 0); // Skeletons stop on wall tiles
    currentPathIndex = 0;
}