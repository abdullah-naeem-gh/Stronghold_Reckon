#include "MapScreen.hpp"
#include <iostream>

MapScreen::MapScreen(int rows, int cols, const sf::Vector2u& windowSize)
    : mapEntity(rows, cols), uiManager(windowSize) {
    cameraView.setSize(static_cast<float>(windowSize.x), static_cast<float>(windowSize.y));

    // Calculate correct center by properly averaging the map size
    sf::Vector2f centerPosition = IsometricUtils::tileToScreen(rows / 2, cols / 2, START_X, START_Y);
    cameraView.setCenter(centerPosition);

    uiManager.loadUI([this](const std::string& buildingTexture) {
        setSelectedBuildingType(buildingTexture);
    });

    std::cout << "MapScreen initialized successfully." << std::endl;
}

void MapScreen::handleEvents(const sf::Event& event, sf::RenderWindow& window) {
    uiManager.handleEvent(event);
    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window), cameraView);
        TileCoordinates tileCoords = IsometricUtils::screenToTile(mousePos.x, mousePos.y, START_X, START_Y);
        auto tile = mapEntity.getTile(tileCoords.row, tileCoords.col);
        if (tile && !selectedBuildingTexture.empty() && !tile->getBuilding()) {
            mapEntity.addBuilding(tileCoords.row, tileCoords.col, selectedBuildingTexture);
        }
    }
    // Remove undo/redo handling here
}

void MapScreen::draw(sf::RenderWindow& window) {
    window.setView(cameraView);
    mapEntity.draw(window);
    window.setView(window.getDefaultView());
    uiManager.draw(window);
}

void MapScreen::setSelectedBuildingType(const std::string& buildingTexture) {
    selectedBuildingTexture = buildingTexture;
}

void MapScreen::moveCamera(const sf::Time& deltaTime) {
    sf::Vector2f movement(0.0f, 0.0f);

    // Move camera using WASD keys
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

    // Apply movement to camera view
    cameraView.move(movement);

    // Clamp the camera view to prevent going out of map bounds
    sf::Vector2f viewSize = cameraView.getSize();
    sf::Vector2f viewCenter = cameraView.getCenter();

    float halfWidth = viewSize.x / 2.0f;
    float halfHeight = viewSize.y / 2.0f;

    // Map boundaries in screen space
    int totalRows = mapEntity.getRows();
    int totalCols = mapEntity.getCols();
    sf::Vector2f furthestTile = IsometricUtils::tileToScreen(totalRows - 1, totalCols - 1, START_X, START_Y);
    float mapWidth = furthestTile.x + Tile::TILE_WIDTH;
    float mapHeight = furthestTile.y + Tile::TILE_HEIGHT;

    if (viewCenter.x - halfWidth < 0) viewCenter.x = halfWidth;
    if (viewCenter.x + halfWidth > mapWidth) viewCenter.x = mapWidth - halfWidth;
    if (viewCenter.y - halfHeight < 0) viewCenter.y = halfHeight;
    if (viewCenter.y + halfHeight > mapHeight) viewCenter.y = mapHeight - halfHeight;

    cameraView.setCenter(viewCenter);
}

void MapScreen::saveMap(const std::string& filename) {
    mapEntity.saveToFile(filename);
}

void MapScreen::loadMap(const std::string& filename) {
    mapEntity.loadFromFile(filename);
}

Map& MapScreen::getMapEntity() {
    return mapEntity;
}