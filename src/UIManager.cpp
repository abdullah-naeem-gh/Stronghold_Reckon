#include "UIManager.hpp"
#include "TextureManager.hpp"
#include <iostream>

UIManager::UIManager(const sf::Vector2u& windowSize) : toolbarHeight(150.0f) {
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

    // Load and position the flame texture
    auto flameTexture = TextureManager::getInstance().getTexture("../assets/ui/toolbar_flame.png");
    if (flameTexture) {
        toolbarFlame.setTexture(*flameTexture);
        float flameScaleY = (toolbarHeight / static_cast<float>(flameTexture->getSize().y)) * 2.5f;
        float flameWidth = flameTexture->getSize().x * flameScaleY;
        toolbarFlame.setScale(flameScaleY, flameScaleY);
        toolbarFlame.setPosition(windowSize.x - flameWidth - 155.0f, windowSize.y - toolbarHeight - 100.0f);
    } else {
        std::cerr << "Toolbar flame texture not loaded." << std::endl;
    }
}

void UIManager::loadUI(const std::function<void(const std::string&, bool)>& selectCallback) {
    auto buildingCallback1 = [=]() {
        selectCallback("../assets/buildings/building1.png", false);
    };
    addButton("../assets/ui/button_building1.png", buildingCallback1);

    auto buildingCallback2 = [=]() {
        selectCallback("../assets/buildings/building2.png", false);
    };
    addButton("../assets/ui/button_building2.png", buildingCallback2);

    auto wallCallback = [=]() {
        selectCallback("../assets/walls/brick_wall.png", false);
    };
    addButton("../assets/ui/button_wall.png", wallCallback);

    auto townHallCallback = [=]() {
        selectCallback("../assets/buildings/townhall.png", false);
    };
    addButton("../assets/ui/button_townhall.png", townHallCallback);

    auto tower1Callback = [=]() {
        selectCallback("../assets/buildings/moontower.png", false);
    };
    addButton("../assets/ui/button_tower1.png", tower1Callback);

    auto barrelTrapCallback = [=]() {
        selectCallback("../assets/traps/BarrelBomb/barrel.png", true);
    };
    addButton("../assets/traps/BarrelBomb/barrel.png", barrelTrapCallback);

    auto mushroomTrapCallback = [=]() {
        selectCallback("../assets/traps/MushroomField/mushrooms1.png", true);
    };
    addButton("../assets/ui/mushrooms11.png", mushroomTrapCallback);
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
    window.draw(toolbarFlame);
}