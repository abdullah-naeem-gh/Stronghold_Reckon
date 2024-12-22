#ifndef BULLET_HPP
#define BULLET_HPP

#include <SFML/Graphics.hpp>
#include <vector>

class Bullet {
public:
    Bullet(sf::Vector2f startPosition, sf::Vector2f targetPosition, float speed);

    Bullet(Bullet &&other) noexcept;

    Bullet &operator=(Bullet &&other) noexcept;

    void update(float deltaTime);
    void render(sf::RenderWindow& window) const;

    bool isActive() const;
    const sf::Vector2f& getPosition() const;

private:
    sf::Vector2f position;
    sf::Vector2f velocity;
    sf::Vector2f targetPosition;
    sf::Sprite sprite;
    bool active;
    float stoppingThreshold = 5.0f;
    
    std::vector<sf::Texture> textures; // Separate textures for each frame
    size_t currentFrame;
    float animationTime;
    const float frameDuration = 0.1f; // Adjust as needed
};

#endif // BULLET_HPP