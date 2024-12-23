#ifndef SKELETON_HPP
#define SKELETON_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include <map>
#include "Tile.hpp"

class Skeleton {
public:
    Skeleton(float x, float y, const std::vector<std::shared_ptr<Tile>>& path);
    void setPosition(float x, float y);
    sf::Vector2f getPosition() const;
    void draw(sf::RenderWindow& window, float deltaTime);
    void move(float deltaTime);

    // **New Methods**
    void takeDamage(int damage);
    bool isAlive() const;
    sf::Sprite& getSprite(); // To access sprite bounds
    bool isDestroyed() const;

    void playExplosionAnimation(float deltaTime);

private:
    sf::Sprite sprite;
    std::map<std::string, std::vector<std::shared_ptr<sf::Texture>>> directionTextures;
    std::vector<std::shared_ptr<Tile>> path;
    size_t currentPathIndex;
    size_t currentAnimationFrame;
    float speed = 70.0f;
    float animationTime = 0.0f;
    const float animationFrameDuration = 0.1f; // Change as suitable for your animation

    void setDirection(float dx, float dy);
    void loadTextures();

    int health = 10; // **New Health Attribute**
    static const int maxHealth = 50;

    // trap
    void checkForTrap(std::shared_ptr<Tile> tile);

    
    // Explosion animation
    std::vector<std::shared_ptr<sf::Texture>> explosionTextures;
    sf::Sprite explosionSprite;
    float explosionTime;
    size_t currentExplosionFrame;
    bool explosionPlaying;
    bool isDead; // Add this flag

};

#endif // SKELETON_HPP