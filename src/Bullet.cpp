#include "Bullet.hpp"
#include <string>
#include <iostream>
#include <cmath>

Bullet::Bullet(sf::Vector2f startPosition, sf::Vector2f targetPosition, float speed)
    : position(startPosition), targetPosition(targetPosition), active(true),
      currentFrame(0), animationTime(0.0f), stoppingThreshold(5.0f) {
    
    // Load each frame separately
    for (int i = 1; i <= 7; ++i) {
        sf::Texture texture;
        if (texture.loadFromFile("../assets/bullets/Moontowerbullet/b" + std::to_string(i) + ".png")) {
            textures.push_back(texture);
        } else {
            std::cerr << "Failed to load bullet frame: b" + std::to_string(i) + ".png" << std::endl;
        }
    }

    if (!textures.empty()) {
        sprite.setTexture(textures[currentFrame]);
        sprite.setOrigin(textures[currentFrame].getSize().x / 2.0f, textures[currentFrame].getSize().y / 2.0f);
    }
    sprite.setPosition(position);

    sf::Vector2f direction = targetPosition - startPosition;
    float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    if (length != 0) {
        velocity = (direction / length) * speed;
    }
}

// Move constructor
Bullet::Bullet(Bullet&& other) noexcept
    : position(std::move(other.position)),
      velocity(std::move(other.velocity)),
      targetPosition(std::move(other.targetPosition)),
      sprite(std::move(other.sprite)),
      active(other.active),
      stoppingThreshold(other.stoppingThreshold),
      textures(std::move(other.textures)),
      currentFrame(other.currentFrame),
      animationTime(other.animationTime) {
    other.active = false;
}

// Move assignment operator
Bullet& Bullet::operator=(Bullet&& other) noexcept {
    if (this != &other) {
        position = std::move(other.position);
        velocity = std::move(other.velocity);
        targetPosition = std::move(other.targetPosition);
        sprite = std::move(other.sprite);
        active = other.active;
        stoppingThreshold = other.stoppingThreshold;
        textures = std::move(other.textures);
        currentFrame = other.currentFrame;
        animationTime = other.animationTime;
        other.active = false;
    }
    return *this;
}


void Bullet::update(float deltaTime) {
    if (active) {
        position += velocity * deltaTime;
        sprite.setPosition(position);

        // Animate the bullet
        animationTime += deltaTime;
        if (animationTime >= frameDuration) {
            animationTime = 0.0f;
            currentFrame = (currentFrame + 1) % textures.size();
            sprite.setTexture(textures[currentFrame]); // Update sprite texture
        }

        // Calculate distance to the target
        sf::Vector2f toTarget = targetPosition - position;
        float distanceToTarget = std::sqrt(toTarget.x * toTarget.x + toTarget.y * toTarget.y);

        if (distanceToTarget <= stoppingThreshold) {
            active = false;
        }
    }
}

void Bullet::render(sf::RenderWindow& window) const {
    if (active) {
        window.draw(sprite);
    }
}

bool Bullet::isActive() const {
    return active;
}

const sf::Vector2f& Bullet::getPosition() const {
    return position;
}