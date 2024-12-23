#ifndef TRAP_HPP
#define TRAP_HPP

#include <SFML/Graphics.hpp>
#include <string>

class Trap {
public:
    Trap(const std::string& texturePath);
    void draw(sf::RenderWindow& window, float x, float y);
    void trigger();
    bool isActive() const;

    std::string getTexturePath();
    int getDamage();

private:
    sf::Sprite sprite;
    sf::Texture texture;
    bool active;
    std::string texturePath;

    int damage;
};

#endif // TRAP_HPP