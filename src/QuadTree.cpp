#include "QuadTree.hpp"

QuadTree::QuadTree(int level, sf::FloatRect bounds) : level(level), bounds(bounds) {}

void QuadTree::clear() {
    entities.clear();
    for (auto& node : nodes) {
        node.clear();
    }
    nodes.clear();
}

void QuadTree::insert(Bullet& bullet) {
    if (!nodes.empty()) {
        int index = getIndex(bullet);
        if (index != -1) {
            nodes[index].insert(bullet);
            return;
        }
    }

    entities.push_back(&bullet);

    if (entities.size() > maxEntities && level < maxLevels) {
        if (nodes.empty()) {
            split();
        }

        size_t i = 0;
        while (i < entities.size()) {
            int index = getIndex(*entities[i]);
            if (index != -1) {
                nodes[index].insert(*entities[i]);
                entities.erase(entities.begin() + i);
            } else {
                i++;
            }
        }
    }
}

std::vector<Bullet*> QuadTree::retrieve(std::vector<Bullet*>& returnEntities, Bullet& bullet) {
    int index = getIndex(bullet);
    if (index != -1 && !nodes.empty()) {
        nodes[index].retrieve(returnEntities, bullet);
    }

    returnEntities.insert(returnEntities.end(), entities.begin(), entities.end());

    return returnEntities;
}

void QuadTree::split() {
    float subWidth = bounds.width / 2.0f;
    float subHeight = bounds.height / 2.0f;
    float x = bounds.left;
    float y = bounds.top;

    nodes.emplace_back(level + 1, sf::FloatRect(x + subWidth, y, subWidth, subHeight));
    nodes.emplace_back(level + 1, sf::FloatRect(x, y, subWidth, subHeight));
    nodes.emplace_back(level + 1, sf::FloatRect(x, y + subHeight, subWidth, subHeight));
    nodes.emplace_back(level + 1, sf::FloatRect(x + subWidth, y + subHeight, subWidth, subHeight));
}

int QuadTree::getIndex(Bullet& bullet) {
    int index = -1;
    float verticalMidpoint = bounds.left + bounds.width / 2.0f;
    float horizontalMidpoint = bounds.top + bounds.height / 2.0f;

    bool topQuadrant = (bullet.getPosition().y < horizontalMidpoint);
    bool bottomQuadrant = (bullet.getPosition().y > horizontalMidpoint);

    if (bullet.getPosition().x < verticalMidpoint) {
        if (topQuadrant) {
            index = 1;
        } else if (bottomQuadrant) {
            index = 2;
        }
    } else if (bullet.getPosition().x > verticalMidpoint) {
        if (topQuadrant) {
            index = 0;
        } else if (bottomQuadrant) {
            index = 3;
        }
    }

    return index;
}