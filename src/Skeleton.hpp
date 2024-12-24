#ifndef SKELETON_HPP
#define SKELETON_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include <map>
#include "Tile.hpp"
#include "Map.hpp" // Add this include for Map reference
#include "Pathfinding.hpp"

class Skeleton {
public:
    Skeleton(float x, float y, const std::vector<std::shared_ptr<Tile>>& path, const Map& map);
    void setPosition(float x, float y);
    sf::Vector2f getPosition() const;
    void draw(sf::RenderWindow& window, float deltaTime);
    void move(float deltaTime);
    // **New or Modified Methods**
    void takeDamage(int damage);
    bool isAlive() const;
    sf::Sprite& getSprite(); 
    bool isDestroyed() const;
    void playExplosionAnimation(float deltaTime);

    void damageWall();

private:
    sf::Sprite sprite;
    std::map<std::string, std::vector<std::shared_ptr<sf::Texture>>> directionTextures;
    std::vector<std::shared_ptr<Tile>> path;
    size_t currentPathIndex;
    size_t currentAnimationFrame;
    float speed = 70.0f;
    float animationTime = 0.0f;
    const float animationFrameDuration = 0.1f;
    void setDirection(float dx, float dy);
    void loadTextures();
    int health = 10;
    static const int maxHealth = 50;

    // **New Member Variables for Wall Interaction**
    const Map& map;  // Reference to the map for pathfinding
    Pathfinding pathFinder;  // Pathfinding utility
    std::shared_ptr<Tile> currentWall;  // Pointer to the current wall being exploded

    // Trap related methods
    void checkForTrap(std::shared_ptr<Tile> tile);

    // Explosion animation
    std::vector<std::shared_ptr<sf::Texture>> explosionTextures;
    sf::Sprite explosionSprite;
    float explosionTime;
    size_t currentExplosionFrame;
    bool explosionPlaying;
    bool isDead;

    // **New Methods for Wall Destruction**
    void explodeWall();
    void recalculatePath();
};

#endif // SKELETON_HPP