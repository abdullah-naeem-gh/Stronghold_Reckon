#ifndef TEXTUREMANAGER_HPP
#define TEXTUREMANAGER_HPP

#include <SFML/Graphics.hpp>
#include <map>
#include <string>
#include <memory>

class TextureManager {
public:
    // Singleton access
    static TextureManager& getInstance();
    // Get individual texture by filename. Returns nullptr if loading fails.
    std::shared_ptr<sf::Texture> getTexture(const std::string& filename);
    // Load the sprite sheet from a file. Returns true on success.
    bool loadSpriteSheet(const std::string& filename);
    // Get the sprite sheet texture. Returns nullptr if not loaded.
    std::shared_ptr<sf::Texture> getSpriteSheet() const;
private:
    TextureManager() = default;
    TextureManager(const TextureManager&) = delete;
    TextureManager& operator=(const TextureManager&) = delete;
    // Map of individual textures
    std::map<std::string, std::shared_ptr<sf::Texture>> textures;
    // Sprite sheet texture
    std::shared_ptr<sf::Texture> spriteSheet;
};

#endif // TEXTUREMANAGER_HPP