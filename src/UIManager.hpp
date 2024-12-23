#ifndef UIMANAGER_HPP
#define UIMANAGER_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include <functional>
#include <string>

struct UIButton {
    sf::Sprite sprite;
    std::function<void()> onClick;
};

class UIManager {
public:
    UIManager(const sf::Vector2u& windowSize);
    void loadUI(const std::function<void(const std::string&, bool)>& selectCallback);
    void handleEvent(const sf::Event& event);
    void draw(sf::RenderWindow& window) const;

private:
    sf::Sprite toolbarBackground;
    sf::Sprite toolbarFlame; 
    std::vector<UIButton> buttons;
    float toolbarHeight;
    void addButton(const std::string& iconPath, const std::function<void()>& callback);
};

#endif // UIMANAGER_HPP