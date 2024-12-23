#include "Tank.hpp"
#include "TextureManager.hpp"
#include <iostream>
#include <cmath>

// Modified Constructor to accept and store path
Tank::Tank(float x, float y, const std::vector<std::shared_ptr<Tile>>& path)
    : currentPathIndex(0),
      health(maxHealth),
      explosionTime(0.0f),
      currentExplosionFrame(0),
      explosionPlaying(false),
      path(path),
      currentState(State::Moving) { // Initialize state to Moving
    loadTextures();
    std::cout << "Tank constructor called at (" << x << ", " << y << ").\n";
    if (!directionTextures.empty()) {
        sprite.setTexture(*directionTextures["left"]);
    }
    sprite.setOrigin(TANK_WIDTH / 2.0f, TANK_HEIGHT);
    sprite.setPosition(x, y);
    sprite.setScale(
        TANK_WIDTH / sprite.getLocalBounds().width,
        TANK_HEIGHT / sprite.getLocalBounds().height
    );

    // Load explosion textures
    for (int i = 1; i <= 10; ++i) {
        std::string explosionPath = "../assets/explosions/Explosion_8/Explosion_" + std::to_string(i) + ".png";
        auto texture = TextureManager::getInstance().getTexture(explosionPath);
        if (texture) {
            explosionTextures.push_back(texture);
        } else {
            std::cerr << "Explosion texture not loaded: " << explosionPath << std::endl;
        }
    }
    if (!explosionTextures.empty()) {
        explosionSprite.setTexture(*explosionTextures[0]);
        explosionSprite.setOrigin(explosionSprite.getLocalBounds().width / 2.0f, explosionSprite.getLocalBounds().height / 2.0f);
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
    for (const auto& [dir, pathStr] : directionPaths) {
        auto texture = TextureManager::getInstance().getTexture(pathStr);
        if (texture) {
            directionTextures[dir] = texture;
        } else {
            std::cerr << "Tank texture not loaded: " << pathStr << std::endl;
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
    if (currentState != State::Destroyed) {
        window.draw(sprite);
    }
    if (explosionPlaying) {
        window.draw(explosionSprite);
    }
}

void Tank::update(float deltaTime) {
    if (currentState == State::Destroyed) {
        if (explosionPlaying) {
            // Assuming explosionTime and currentExplosionFrame are managed here
            // playExplosionAnimation(deltaTime); // Uncomment if implemented
        }
        return;
    }
    switch (currentState) {
        case State::Moving:
            moveTank(deltaTime);
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
        case State::Destroyed:
            // Handle destruction state if needed
            break;
    }
}

void Tank::moveTank(float deltaTime) { // Renamed to moveTank
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
            if (currentPathIndex >= path.size()) {
                currentState = State::Resting;
            }
        }
    }
}

void Tank::takeDamage(int damage) {
    health -= damage;
    if (health <= 0) {
        health = 0;
        std::cout << "Tank destroyed at position ("
                  << sprite.getPosition().x << ", "
                  << sprite.getPosition().y << ").\n";
        currentState = State::Destroyed;
        explosionPlaying = true;
        explosionTime = 0.0f;
        currentExplosionFrame = 0;
        explosionSprite.setPosition(sprite.getPosition());
    } else {
        std::cout << "Tank took " << damage
                  << " damage, remaining health: " << health << ".\n";
    }
}

bool Tank::isAlive() const {
    return health > 0;
}

sf::Sprite& Tank::getSprite() {
    return sprite;
}

void Tank::setDirection(float dx, float dy) {
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
        sprite.setTexture(*directionTextures.at(directionKey));
    }
}

void Tank::attackWall(float deltaTime) {
    if (wallTile && wallTile->getHealth() > 0) {
        wallTile->takeDamage(static_cast<int>(3 * deltaTime)); // Example damage value
    } else {
        if (wallTile) wallTile->setBlockStatus(false);
        currentState = State::RecalculatingPath;
    }
}

void Tank::recalculatePath() {
    // Implement path recalculation logic here if needed
    // For example, find a new path to the town hall
    // This requires access to the Map class or Pathfinding instance
}

void Tank::rest() {
    // Tank is resting, no actions needed
}

void Tank::playExplosionAnimation(float deltaTime) {
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
}

void Tank::checkForTrap(std::shared_ptr<Tile> tile) {
    if (tile->getTrap() && tile->getTrap()->isActive()) {
        takeDamage(tile->getTrap()->getDamage()); // Example damage value
        tile->getTrap()->trigger();
    }
}