// MapScreen.cpp
#include "MapScreen.hpp"
#include "IsometricUtils.hpp"
#include <algorithm>
#include <iostream>

MapScreen::MapScreen(int rows, int cols, const sf::Vector2u& windowSize)
    : mapEntity(rows, cols), // Initialize mapEntity first
      uiManager(windowSize),
      tankSpawn(mapEntity) {
    cameraView.setSize(static_cast<float>(windowSize.x), static_cast<float>(windowSize.y));
    // Calculate correct center by properly averaging the map size
    sf::Vector2f centerPosition = IsometricUtils::tileToScreen(rows / 2, cols / 2);
    cameraView.setCenter(centerPosition);
    uiManager.loadUI([this](const std::string& buildingTexture) {
        setSelectedBuildingType(buildingTexture);
    });
    std::cout << "MapScreen initialized successfully." << std::endl;
}

void MapScreen::handleEvents(const sf::Event& event, sf::RenderWindow& window) {
    uiManager.handleEvent(event);
    // Handle tank spawning within MapScreen
    tankSpawn.handleEvent(event, mapEntity);

    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        // Ensure mouse position conversion accounts for the camera view
        sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window), cameraView);
        TileCoordinates tileCoords = IsometricUtils::screenToTile(mousePos.x, mousePos.y, mapEntity.getRows(), mapEntity.getCols());
        auto tile = mapEntity.getTile(tileCoords.row, tileCoords.col);
        if (tile && !selectedBuildingTexture.empty() && !tile->getBuilding()) {
            mapEntity.addBuilding(tileCoords.row, tileCoords.col, selectedBuildingTexture);
        }
    }
}

void MapScreen::draw(sf::RenderWindow& window, float deltaTime) {
    window.setView(cameraView);
    mapEntity.draw(window);
    tankSpawn.draw(window, deltaTime); // Draw tanks within camera view
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

    // Accurate Map Boundary Calculations
    // Calculate leftmost and rightmost tile positions
    sf::Vector2f leftMostTile = IsometricUtils::tileToScreen(mapEntity.getRows() - 1, 0); // (row, col=0)
    sf::Vector2f rightMostTile = IsometricUtils::tileToScreen(0, mapEntity.getCols() - 1); // (row=0, col)

    // Calculate map boundaries
    float mapLeft = leftMostTile.x;
    float mapRight = rightMostTile.x + Tile::TILE_WIDTH;
    float mapTop = IsometricUtils::getMapStartY();
    float mapBottom = mapRight; // Not used in horizontal movement

    // Get current view size and center
    sf::Vector2f viewSize = cameraView.getSize();
    sf::Vector2f viewCenter = cameraView.getCenter();
    float halfWidth = viewSize.x / 2.0f;
    float halfHeight = viewSize.y / 2.0f;

    // Clamp X axis to prevent the camera from moving beyond map boundaries
    if (viewCenter.x - halfWidth < mapLeft) {
        viewCenter.x = mapLeft + halfWidth;
    }
    if (viewCenter.x + halfWidth > mapRight) {
        viewCenter.x = mapRight - halfWidth;
    }

    // Clamp Y axis to prevent the camera from moving beyond map boundaries
    // Calculate mapHeight based on tileToScreen for (rows - 1, cols -1)
    sf::Vector2f furthestTile = IsometricUtils::tileToScreen(mapEntity.getRows() - 1, mapEntity.getCols() - 1);
    float mapHeight = furthestTile.y + Tile::TILE_HEIGHT;

    if (viewCenter.y - halfHeight < mapTop) {
        viewCenter.y = mapTop + halfHeight;
    }
    if (viewCenter.y + halfHeight > mapHeight) {
        viewCenter.y = mapHeight - halfHeight;
    }

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