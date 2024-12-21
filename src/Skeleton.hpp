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
    static const int TILE_WIDTH = 64;
    static const int TILE_HEIGHT = 32;

private:
    sf::Sprite sprite;
    std::map<std::string, std::vector<std::shared_ptr<sf::Texture>>> directionTextures;
    std::vector<std::shared_ptr<Tile>> path;
    size_t currentPathIndex;
    size_t currentAnimationFrame;
    float speed = 100.0f;
    float animationTime = 0.0f;
    const float animationFrameDuration = 0.1f; // Change as suitable for your animation

    void setDirection(float dx, float dy);
    void loadTextures();
};

#endif // SKELETON_HPP