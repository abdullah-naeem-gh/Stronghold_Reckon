#include "TextureManager.hpp"
#include <iostream>

TextureManager& TextureManager::getInstance() {
    static TextureManager instance;
    return instance;
}

std::shared_ptr<sf::Texture> TextureManager::getTexture(const std::string& filename) {
    // Check if texture is already loaded
    auto it = textures.find(filename);
    if (it != textures.end()) {
        return it->second;
    }
    // Load the texture
    auto texture = std::make_shared<sf::Texture>();
    if (!texture->loadFromFile(filename)) {
        std::cerr << "Failed to load texture: " << filename << std::endl;
        return nullptr;
    }
    // Store and return the texture
    textures[filename] = texture;
    return texture;
}

bool TextureManager::loadSpriteSheet(const std::string& filename) {
    // Check if sprite sheet is already loaded
    if (spriteSheet && spriteSheet->getSize().x > 0 && spriteSheet->getSize().y > 0) {
        return true; // Already loaded
    }
    // Load the sprite sheet
    spriteSheet = std::make_shared<sf::Texture>();
    if (!spriteSheet->loadFromFile(filename)) {
        std::cerr << "Failed to load sprite sheet: " << filename << std::endl;
        return false;
    }
    return true;
}

std::shared_ptr<sf::Texture> TextureManager::getSpriteSheet() const {
    return spriteSheet;
}