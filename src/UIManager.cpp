#include "UIManager.hpp"
#include "TextureManager.hpp"
#include <iostream>

UIManager::UIManager(const sf::Vector2u& windowSize) : toolbarHeight(150.0f) { // Adjusted toolbar height
    auto bgTexture = TextureManager::getInstance().getTexture("../assets/ui/toolbar_background.png");
    if (bgTexture) {
        toolbarBackground.setTexture(*bgTexture);
        toolbarBackground.setPosition(-300, windowSize.y - toolbarHeight);
        
        // Adjust scaling for vertical stretch
        float scaleX = static_cast<float>(windowSize.x) / static_cast<float>(bgTexture->getSize().x);
        float scaleY = toolbarHeight / static_cast<float>(bgTexture->getSize().y);
        toolbarBackground.setScale(scaleX, scaleY);
    } else {
        std::cerr << "Toolbar background texture not loaded." << std::endl;
    }

    // Restore the flame texture and position
    auto flameTexture = TextureManager::getInstance().getTexture("../assets/ui/toolbar_flame.png");
    if (flameTexture) {
        toolbarFlame.setTexture(*flameTexture);

        // Scale the flame size (increased size multiplier for toolbar height)
        float flameScaleY = (toolbarHeight / static_cast<float>(flameTexture->getSize().y)) * 2.5f; // Increased size multiplier for the toolbar height
        float flameWidth = flameTexture->getSize().x * flameScaleY;

        toolbarFlame.setScale(flameScaleY, flameScaleY);
        // Adjusted the flame position to remain fixed relative to the toolbar
        toolbarFlame.setPosition(windowSize.x - flameWidth - 155.0f, windowSize.y - toolbarHeight - 100.0f); 
    } else {
        std::cerr << "Toolbar flame texture not loaded." << std::endl;
    }
}

void UIManager::loadUI(const std::function<void(const std::string&)>& buildingSelectCallback) {
    auto callback1 = [=]() {
        buildingSelectCallback("../assets/buildings/building1.png");
    };
    addButton("../assets/ui/button_building1.png", callback1);
    
    auto callback2 = [=]() {
        buildingSelectCallback("../assets/buildings/building2.png");
    };
    addButton("../assets/ui/button_building2.png", callback2);
    
    auto wallCallback = [=]() {
        buildingSelectCallback("../assets/walls/brick_wall.png");
    };
    addButton("../assets/ui/button_wall.png", wallCallback);
    
    auto townHallCallback = [=]() {
        buildingSelectCallback("../assets/buildings/townhall.png");
    };
    addButton("../assets/ui/button_townhall.png", townHallCallback);
    
    auto tower1Callback = [=]() {
        buildingSelectCallback("../assets/buildings/moontower.png");
    };
    addButton("../assets/ui/button_tower1.png", tower1Callback);
}

void UIManager::addButton(const std::string& iconPath, const std::function<void()>& callback) {
    auto texture = TextureManager::getInstance().getTexture(iconPath);
    if (texture) {
        UIButton button;
        button.sprite.setTexture(*texture);
        float padding = 10.0f;
        float buttonSize = 64.0f;
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
    } else {
        std::cerr << "Button icon failed to load: " << iconPath << std::endl;
    }
}

void UIManager::handleEvent(const sf::Event& event) {
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
    window.draw(toolbarFlame); // Ensure the flame is drawn
}