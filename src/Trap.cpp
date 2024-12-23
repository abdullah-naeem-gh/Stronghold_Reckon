#include "Trap.hpp"
#include <iostream>

Trap::Trap(const std::string& texturePath) : active(true) {
    if (!texture.loadFromFile(texturePath)) {
        std::cerr << "Failed to load trap texture: " << texturePath << std::endl;
    }
    sprite.setTexture(texture);
    this->texturePath = texturePath;
    if (texturePath == "../assets/traps/BarrelBomb/barrel.png") {
        damage = 100;
    } else if (texturePath == "../assets/traps/MushroomField/mushrooms1.png") {
        damage = 20;
    } else {
        damage = 0;
    }
    // std::cout << "Trap constructor called, active status " << active << ".\n";
}

void Trap::draw(sf::RenderWindow& window, float x, float y) {
    if (active) {
        sprite.setPosition(x, y);
        window.draw(sprite);
        // std::cout << "Trap drawn at (" << x << ", " << y << ").\n";
    }
}

void Trap::trigger() {
    if (active) {
        std::cout << "Trap triggered.\n";
        active = false;
    }
}

bool Trap::isActive() const {
    return active;
}


std::string Trap::getTexturePath() {
    return texturePath;
}


int Trap::getDamage() {
    return damage;
}