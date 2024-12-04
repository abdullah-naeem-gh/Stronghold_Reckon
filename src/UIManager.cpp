// UIManager.cpp
#include "UIManager.hpp"
#include "TextureManager.hpp"
#include <iostream>

UIManager::UIManager(const sf::Vector2u& windowSize) : toolbarHeight(100.0f) { // Adjust toolbar height as needed
    // Load toolbar background
    auto bgTexture = TextureManager::getInstance().getTexture("../assets/ui/toolbar_background.png");
    if (bgTexture) {
        toolbarBackground.setTexture(*bgTexture);
        toolbarBackground.setPosition(0, windowSize.y - toolbarHeight);
        // Scale the background to fit the window width
        float scaleX = static_cast<float>(windowSize.x) / static_cast<float>(bgTexture->getSize().x);
        float scaleY = toolbarHeight / static_cast<float>(bgTexture->getSize().y);
        toolbarBackground.setScale(scaleX, scaleY);
    }
    else {
        std::cerr << "Toolbar background texture not loaded." << std::endl;
    }
}

void UIManager::loadUI(const std::function<void(const std::string&)>& buildingSelectCallback) {
    // Example: Add Building Type 1 Button
    // Pass the building texture path as a parameter to the callback
    auto callback1 = [=]() {
        buildingSelectCallback("../assets/buildings/building1.png");
    };
    addButton("../assets/ui/button_building1.png", callback1);

    // Add Building Type 2 Button
    auto callback2 = [=]() {
        buildingSelectCallback("../assets/buildings/building2.png");
    };
    addButton("../assets/ui/button_building2.png", callback2);

    // Add Wall Button
    auto wallCallback = [=]() {
        buildingSelectCallback("../assets/walls/brick_wall.png");
    };
    addButton("../assets/ui/button_wall.png", wallCallback);

    // Add Town Hall Button
    auto townHallCallback = [=]() {
        buildingSelectCallback("../assets/buildings/townhall.png");
    };
    addButton("../assets/ui/button_townhall.png", townHallCallback);

    // Add Town Hall Button
    auto tower1Callback = [=]() {
        buildingSelectCallback("../assets/buildings/tower1.png");
    };
    addButton("../assets/ui/button_tower1.png", tower1Callback);
    // Add more buttons as needed
}

void UIManager::addButton(const std::string& iconPath, const std::function<void()>& callback) {
    auto texture = TextureManager::getInstance().getTexture(iconPath);
    if (texture) {
        UIButton button;
        button.sprite.setTexture(*texture);
        // Position buttons horizontally with some padding within the toolbar
        float padding = 10.0f;
        float buttonSize = 64.0f; // Assuming buttons are 64x64 pixels
        size_t index = buttons.size();
        button.sprite.setPosition(
            padding + index * (buttonSize + padding),
            toolbarBackground.getPosition().y + (toolbarBackground.getGlobalBounds().height - buttonSize) / 2.0f
        );
        button.sprite.setScale(
            buttonSize / static_cast<float>(texture->getSize().x),
            buttonSize / static_cast<float>(texture->getSize().y)
        );
        button.onClick = callback;
        buttons.push_back(button);
    }
    else {
        std::cerr << "Button icon failed to load: " << iconPath << std::endl;
    }
}

void UIManager::handleEvent(const sf::Event& event) { // **Removed window parameter**
    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        sf::Vector2f mousePos(static_cast<float>(event.mouseButton.x), static_cast<float>(event.mouseButton.y));
        for (const auto& button : buttons) {
            if (button.sprite.getGlobalBounds().contains(mousePos)) {
                if (button.onClick) {
                    std::cout << "Button clicked: Executing callback.\n";
                    button.onClick();
                }
            }
        }
    }
}

void UIManager::draw(sf::RenderWindow& window) const {
    window.draw(toolbarBackground);
    for (const auto& button : buttons) {
        window.draw(button.sprite);
    }
}