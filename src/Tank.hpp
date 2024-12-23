#ifndef TANK_HPP
#define TANK_HPP

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include <memory>
#include <map>
#include "Tile.hpp"
#include "IsometricUtils.hpp"
#include "Pathfinding.hpp"
#include "Trap.hpp"

class Tank {
public:
    // Modified Constructor to accept path
    Tank(float x, float y, const std::vector<std::shared_ptr<Tile>>& path);

    void setPosition(float x, float y);
    sf::Vector2f getPosition() const;
    void draw(sf::RenderWindow& window) const;
    void update(float deltaTime);
    void takeDamage(int damage);
    bool isAlive() const;
    sf::Sprite& getSprite(); // To access sprite for collision

    static const int TANK_WIDTH = 64;
    static const int TANK_HEIGHT = 64;

private:
    sf::Sprite sprite;
    size_t currentPathIndex;
    std::map<std::string, std::shared_ptr<sf::Texture>> directionTextures;
    void setDirection(float dx, float dy);
    void loadTextures();
    float speed = 100.0f;
    int health;
    static const int maxHealth = 10;
    TileCoordinates townHall = {14, 14};
    std::vector<std::shared_ptr<Tile>> path; // Store the path
    enum class State {
        Moving,
        AttackingWall,
        RecalculatingPath,
        Resting,
        Destroyed
    };
    State currentState;
    std::shared_ptr<Tile> wallTile;
    std::vector<std::shared_ptr<sf::Texture>> explosionTextures;
    sf::Sprite explosionSprite;
    float explosionTime;
    size_t currentExplosionFrame;
    bool explosionPlaying;

    // State-specific methods
    void attackWall(float deltaTime);
    void recalculatePath();
    void rest();
    void playExplosionAnimation(float deltaTime);
    void checkForTrap(std::shared_ptr<Tile> tile);

    // Renamed movement method to avoid recursion
    void moveTank(float deltaTime);
};

#endif // TANK_HPP