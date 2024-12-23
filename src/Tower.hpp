#ifndef TOWER_HPP
#define TOWER_HPP

#include <SFML/Graphics.hpp>
#include <vector>
// Forward declare BulletManager
class BulletManager;

class Tower {
public:
    Tower(int id, sf::Vector2f position, float range, float fireRate, BulletManager& centralBulletManager, const std::string& texturePath);
    void update(float deltaTime, const std::vector<sf::Vector2f>& troopPositions);
    void render(sf::RenderWindow& window) const;
    bool isWithinRange(sf::Vector2f troopPosition) const;

    int getId() const;
    std::string getTexturePath() const;

private:
    int id;
    std::string texturePath;

    sf::Vector2f position;
    float range;
    float fireRate;
    float timeSinceLastShot;
    BulletManager& bulletManager; // Reference to centralized BulletManager
    sf::Sprite towerSprite;
    void fireAt(sf::Vector2f target);
};

#endif // TOWER_HPP