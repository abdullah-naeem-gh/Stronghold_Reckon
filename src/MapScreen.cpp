// MapScreen.cpp
#include "MapScreen.hpp"
#include <iostream>

MapScreen::MapScreen(int rows, int cols, const sf::Vector2u& windowSize)
    : mapEntity(rows, cols), uiManager(windowSize) {
    // Pass a callback to UIManager to handle building selection
    uiManager.loadUI([this](const std::string& buildingTexture) {
        setSelectedBuildingType(buildingTexture);
    });
}

void MapScreen::handleEvents(const sf::Event& event, sf::RenderWindow& window) {
    uiManager.handleEvent(event, window);

    if (event.type == sf::Event::MouseButtonPressed &&
        event.mouseButton.button == sf::Mouse::Left) {
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        float x = static_cast<float>(mousePos.x);
        float y = static_cast<float>(mousePos.y);

        // Check if the click is outside the toolbar
        if (y < static_cast<float>(window.getSize().y) - 100.0f) { // Assuming toolbarHeight = 100
            if (!selectedBuildingTexture.empty()) {
                // Convert screen (x, y) to isometric (row, col) using utility
                TileCoordinates tc = IsometricUtils::screenToTile(x, y);
                int row = tc.row;
                int col = tc.col;

                // Validate tile coordinates
                auto tile = mapEntity.getTile(row, col);
                if (tile) {
                    // Avoid placing buildings on non-grass tiles if needed
                    if (tile->getType() == TileType::Grass) {
                        auto building = mapEntity.addBuilding(row, col, selectedBuildingTexture);
                        if (building) {
                            std::cout << "Building added at Row " << row << ", Col " << col << std::endl;
                        }
                    } else {
                        std::cout << "Cannot place building on this tile type." << std::endl;
                    }
                } else {
                    std::cout << "Clicked outside the map boundaries." << std::endl;
                }
            }
        }
    }
}

void MapScreen::draw(sf::RenderWindow& window) {
    mapEntity.draw(window);
    uiManager.draw(window);
}

void MapScreen::setSelectedBuildingType(const std::string& buildingTexture) {
    selectedBuildingTexture = buildingTexture;
    std::cout << "Selected Building Texture: " << buildingTexture << std::endl;
}