#ifndef TEXTUREMANAGER_HPP
#define TEXTUREMANAGER_HPP

#include <SFML/Graphics.hpp>
#include <map>
#include <string>
#include <memory>

class TextureManager {
public:
    static TextureManager& getInstance();

    // Get texture by filename. Returns nullptr if loading fails.
    std::shared_ptr<sf::Texture> getTexture(const std::string& filename);

private:
    TextureManager() = default;
    TextureManager(const TextureManager&) = delete;
    TextureManager& operator=(const TextureManager&) = delete;

    std::map<std::string, std::shared_ptr<sf::Texture>> textures;
};

#endif // TEXTUREMANAGER_HPP