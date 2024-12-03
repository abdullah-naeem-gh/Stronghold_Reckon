// MapScreen.cpp
#include "MapScreen.hpp"
#include <iostream>

MapScreen::MapScreen(int rows, int cols, const sf::Vector2u& windowSize)
    : mapEntity(rows, cols), uiManager(windowSize) {
    // **Define START_X and START_Y consistently**
    const float START_X = 400.0f; // Must match Map.cpp's START_X
    const float START_Y = 50.0f;  // Must match Map.cpp's START_Y
    
    // Initialize view
    cameraView.setSize(static_cast<float>(windowSize.x), static_cast<float>(windowSize.y));

    // Center the view around the center of the map
    int centerRow = mapEntity.getRows() / 2;
    int centerCol = mapEntity.getCols() / 2;
    sf::Vector2f centerPosition = IsometricUtils::tileToScreen(centerRow, centerCol, START_X, START_Y);
    cameraView.setCenter(centerPosition);

    // Pass a callback to UIManager to handle building selection
    uiManager.loadUI([this](const std::string& buildingTexture) {
        setSelectedBuildingType(buildingTexture);
    });
}

void MapScreen::handleEvents(const sf::Event& event, sf::RenderWindow& window) {
    uiManager.handleEvent(event);

    const float START_X = 400.0f;
    const float START_Y = 50.0f;

    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    sf::Vector2f mouseWorldPos = window.mapPixelToCoords(mousePos, cameraView);

    TileCoordinates tc = IsometricUtils::screenToTile(mouseWorldPos.x, mouseWorldPos.y, START_X, START_Y);
    int row = tc.row;
    int col = tc.col;

    int totalRows = mapEntity.getRows();
    int totalCols = mapEntity.getCols();

    // Offset to target correct tiles for placing and highlighting
    if (row >= 0 && row < totalRows - 1 && col >= 0 && col < totalCols - 1) {
        highlightTile1 = mapEntity.getTile(row, col);
        highlightTile2 = mapEntity.getTile(row + 1, col + 1);

        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
            if (!selectedBuildingTexture.empty()) {
                auto building = mapEntity.addBuilding(row, col, selectedBuildingTexture);
                if (building) {
                    // std::cout << "Building added at Row " << row << ", Col " << col << std::endl;
                }
            }
        }
    } else {
        highlightTile1 = nullptr;
        highlightTile2 = nullptr;
    }
}

void MapScreen::draw(sf::RenderWindow& window) {
    // Set the current view (camera)
    window.setView(cameraView);

    // Draw the map
    mapEntity.draw(window);

    // Commenting out the highlight logic for now
    /*
    if (highlightTile1) {
        sf::RectangleShape highlightRect(sf::Vector2f(Tile::TILE_WIDTH, Tile::TILE_HEIGHT));
        highlightRect.setFillColor(sf::Color(255, 255, 0, 100)); // Semi-transparent yellow
        highlightRect.setPosition(highlightTile1->getPosition());
        window.draw(highlightRect);
    }

    if (highlightTile2) {
        sf::RectangleShape highlightRect(sf::Vector2f(Tile::TILE_WIDTH, Tile::TILE_HEIGHT));
        highlightRect.setFillColor(sf::Color(255, 255, 0, 100)); // Semi-transparent yellow
        highlightRect.setPosition(highlightTile2->getPosition());
        window.draw(highlightRect);
    }
    */

    // Reset to default view to draw UI on top
    window.setView(window.getDefaultView());

    // Draw the UI
    uiManager.draw(window);
}

void MapScreen::setSelectedBuildingType(const std::string& buildingTexture) {
    selectedBuildingTexture = buildingTexture;
    std::cout << "Selected Building Texture: " << buildingTexture << std::endl;
}

void MapScreen::moveCamera(const sf::Time& deltaTime) { // Corrected signature
    // **Use the same START_X and START_Y**
    const float START_X = 400.0f; // Must match Map.cpp's START_X
    const float START_Y = 50.0f;  // Must match Map.cpp's START_Y

    // Determine movement direction based on keyboard input
    sf::Vector2f movement(0.0f, 0.0f);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
        movement.y -= cameraSpeed * deltaTime.asSeconds();
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
        movement.y += cameraSpeed * deltaTime.asSeconds();
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
        movement.x -= cameraSpeed * deltaTime.asSeconds();
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        movement.x += cameraSpeed * deltaTime.asSeconds();
    }

    // Move the view
    cameraView.move(movement);

    // Clamp the view to the map boundaries
    sf::Vector2f viewSize = cameraView.getSize();
    sf::Vector2f viewCenter = cameraView.getCenter();
    float halfWidth = viewSize.x / 2.0f;
    float halfHeight = viewSize.y / 2.0f;

    // Calculate map bounds in screen space
    int totalRows = mapEntity.getRows();
    int totalCols = mapEntity.getCols();

    // Calculate the furthest tile's screen position
    sf::Vector2f furthestTile = IsometricUtils::tileToScreen(totalRows - 1, totalCols - 1, START_X, START_Y);
    float mapWidth = furthestTile.x + Tile::TILE_WIDTH;
    float mapHeight = furthestTile.y + Tile::TILE_HEIGHT;

    // Clamp viewCenter.x
    if (viewCenter.x - halfWidth < 0)
        viewCenter.x = halfWidth;
    if (viewCenter.x + halfWidth > mapWidth)
        viewCenter.x = mapWidth - halfWidth;

    // Clamp viewCenter.y
    if (viewCenter.y - halfHeight < 0)
        viewCenter.y = halfHeight;
    if (viewCenter.y + halfHeight > mapHeight)
        viewCenter.y = mapHeight - halfHeight;

    cameraView.setCenter(viewCenter);
}

void MapScreen::saveMap(const std::string& filename) {
    mapEntity.saveToFile(filename);
}

void MapScreen::loadMap(const std::string& filename) {
    mapEntity.loadFromFile(filename);
}