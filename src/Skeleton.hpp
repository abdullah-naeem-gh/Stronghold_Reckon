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

private:
    sf::Sprite sprite;
    std::map<std::string, std::vector<std::shared_ptr<sf::Texture>>> directionTextures;
    std::vector<std::shared_ptr<Tile>> path;
    size_t currentPathIndex;
    size_t currentAnimationFrame;
    float speed = 70.0f;
    float animationTime = 0.0f;
    const float animationFrameDuration = 0.1f; // Change as suitable for your animation
    int health = 10; // **New Health Attribute**

    void setDirection(float dx, float dy);
    void loadTextures();
};

#endif // SKELETON_HPP