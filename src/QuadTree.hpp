#ifndef QUADTREE_HPP
#define QUADTREE_HPP

#include <vector>
#include <SFML/Graphics.hpp>
#include "Bullet.hpp"

class QuadTree {
public:
    QuadTree(int level, sf::FloatRect bounds);
    void clear();
    void insert(Bullet& bullet);
    std::vector<Bullet*> retrieve(std::vector<Bullet*>& returnEntities, Bullet& bullet);

private:
    int maxEntities = 10;
    int maxLevels = 5;
    int level;
    std::vector<Bullet*> entities;
    sf::FloatRect bounds;
    std::vector<QuadTree> nodes;

    void split();
    int getIndex(Bullet& bullet);
};

#endif // QUADTREE_HPP