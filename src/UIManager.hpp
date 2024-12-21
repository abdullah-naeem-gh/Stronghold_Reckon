// UIManager.hpp
#ifndef UIMANAGER_HPP
#define UIMANAGER_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include <functional>
#include <string>
#include "Tile.hpp"

struct UIButton {
    sf::Sprite sprite;
    std::function<void()> onClick;
};

class UIManager {
public:
    UIManager(const sf::Vector2u& windowSize);

    // Load UI elements with a callback for building selection
    void loadUI(const std::function<void(const std::string&, TileType)>& buildingSelectCallback);

    // Process events related to UI
    void handleEvent(const sf::Event& event); // **Removed sf::RenderWindow& window**

    // Draw UI elements
    void draw(sf::RenderWindow& window) const;

private:
    sf::Sprite toolbarBackground;
    std::vector<UIButton> buttons;
    float toolbarHeight;

    // Correctly declare addButton
    void addButton(const std::string& iconPath, const std::function<void()>& callback);
};

#endif // UIMANAGER_HPP