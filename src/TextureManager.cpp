// TextureManager.cpp
   #include "TextureManager.hpp"
   #include <iostream>

   TextureManager& TextureManager::getInstance() {
       static TextureManager instance;
       return instance;
   }

   std::shared_ptr<sf::Texture> TextureManager::getTexture(const std::string& filename) {
       auto it = textures.find(filename);
       if (it != textures.end()) {
           return it->second;
       }
       auto texture = std::make_shared<sf::Texture>();
       if (!texture->loadFromFile(filename)) {
           std::cerr << "Failed to load texture: " << filename << std::endl;
           return nullptr;
       }
       textures[filename] = texture;
       return texture;
   }