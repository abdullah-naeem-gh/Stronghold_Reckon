#ifndef TANK_HPP
#define TANK_HPP

#include "Tile.hpp"
#include "Map.hpp"
#include "Pathfinding.hpp"
#include "Trap.hpp"
#include <memory>
#include <vector>
#include <SFML/Graphics.hpp>

class Tank {
public:
    Tank(float x, float y, const Map& map, const Tile& townHall);
    void update(float deltaTime);
    void draw(sf::RenderWindow& window) const;
    void takeDamage(int damage, float deltaTime);
    bool isDestroyed() const;

    sf::Vector2f getPosition() const;
    sf::Sprite getSprite() const;

private:
    enum class State {
        Moving,
        AttackingWall,
        RecalculatingPath,
        Resting,
        Destroyed
    };

    void move(float deltaTime);
    void attackWall(float deltaTime);
    void recalculatePath();
    void rest();
    void playExplosionAnimation(float deltaTime);

    sf::Sprite sprite;
    const Map& map;
    const Tile& townHall;
    Pathfinding pathFinder;
    std::vector<std::shared_ptr<Tile>> path;
    size_t currentPathIndex;
    State currentState;
    float speed;
    std::shared_ptr<Tile> wallTile;

    int health;
    static const int maxHealth = 100;

    void setDirection(float dx, float dy);
    void loadTextures();

    static const int TANK_WIDTH = 64;
    static const int TANK_HEIGHT = 64;
    std::map<std::string, std::shared_ptr<sf::Texture>> directionTextures;

    void checkForTrap(std::shared_ptr<Tile> tile);

    // Explosion animation
    std::vector<std::shared_ptr<sf::Texture>> explosionTextures;
    sf::Sprite explosionSprite;
    float explosionTime;
    size_t currentExplosionFrame;
    bool explosionPlaying;
};

#endif // TANK_HPP