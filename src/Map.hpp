#ifndef MAP_HPP
#define MAP_HPP

#include <SFML/Graphics.hpp>

class Map {
public:
    Map();
    void draw(sf::RenderWindow &window);

private:
    sf::Texture mapTexture;
    sf::Sprite mapSprite;
    void loadMap();
};

#endif // MAP_HPP