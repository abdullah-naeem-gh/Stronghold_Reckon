#include "Skeleton.hpp"
#include "TextureManager.hpp"
#include <cmath>
#include <iostream>
#include "Tile.hpp"

Skeleton::Skeleton(float x, float y, const std::vector<std::shared_ptr<Tile>>& path)
    : path(path), currentPathIndex(0), currentAnimationFrame(0), health(10) {
   
    loadTextures();
    if (!directionTextures["left"].empty()) {
        sprite.setTexture(*directionTextures["left"][0]); // Initialize with the first frame for left
    }

    // Set the texture rectangle to focus on the central 64x64 area
    sf::IntRect textureRect(96, 96, 64, 64); // Adjust based on the position of the skeleton in the image
    sprite.setTextureRect(textureRect);

    // Adjust origin to the center of this 64x64 area
    sprite.setOrigin(32.0f, 32.0f);

    // Optionally adjust scale if needed
    sprite.setScale(1.0f, 1.0f); // Adjust if the skeleton needs to be rendered smaller or larger in-game

    sprite.setPosition(x, y);
    
    // Load explosion textures
    for (int i = 1; i <= 8; ++i) {
        std::string path = "../assets/explosions/Explosion_8/Explosion_" + std::to_string(i) + ".png";
        auto texture = TextureManager::getInstance().getTexture(path);
        if (texture) {
            explosionTextures.push_back(texture);
        } else {
            std::cerr << "Explosion texture not loaded: " << path << std::endl;
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
            std::string path = basePath + folder + "skeleton_" + dir + "_" + std::to_string(i) + ".png";
            auto texture = TextureManager::getInstance().getTexture(path);
            if (texture) {
                textures.push_back(texture);
            } else {
                std::cerr << "Skeleton texture not loaded: " << path << std::endl;
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
     // window.draw(sprite);
    if (!explosionPlaying && !isDead) {
        window.draw(sprite);
    }
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
        sf::Vector2f targetPos = path[currentPathIndex]->getPosition();
        sf::Vector2f direction = targetPos - sprite.getPosition();
        float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);
        if (distance > 1.0f) {
            direction /= distance;
            setDirection(direction.x, direction.y);
            sprite.move(direction * speed * deltaTime);
        } else {
            checkForTrap(path[currentPathIndex]);
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
        std::cout << "Skeleton triggered a trap at (" << tile->getRow() << ", " << tile->getCol() << ").\n";
        takeDamage(tile->getTrap()->getDamage()); // Example damage value
        tile->getTrap()->trigger();
    }
}

void Skeleton::playExplosionAnimation(float deltaTime) {
    if (explosionPlaying) {
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
    // delete this;
}