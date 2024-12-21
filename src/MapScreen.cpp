#include "MapScreen.hpp"
#include "IsometricUtils.hpp"
#include <iostream>

MapScreen::MapScreen(int rows, int cols, const sf::Vector2u& windowSize)
    : mapEntity(rows, cols),
      uiManager(windowSize),
      tankSpawn(mapEntity) {
    cameraView.setSize(static_cast<float>(windowSize.x), static_cast<float>(windowSize.y));
    sf::Vector2f centerPosition = IsometricUtils::tileToScreen(rows / 2, cols / 2);
    cameraView.setCenter(centerPosition);
    uiManager.loadUI([this](const std::string& buildingTexture) {
        setSelectedBuildingType(buildingTexture);
    });
}

void MapScreen::handleEvents(const sf::Event& event, sf::RenderWindow& window) {
    uiManager.handleEvent(event);
    tankSpawn.handleEvent(event, mapEntity);
    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
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
    tankSpawn.draw(window, deltaTime);
    window.setView(window.getDefaultView());
    uiManager.draw(window);
}

void MapScreen::setSelectedBuildingType(const std::string& buildingTexture) {
    selectedBuildingTexture = buildingTexture;
}

void MapScreen::moveCamera(const sf::Time& deltaTime) {
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
    cameraView.move(movement);

    sf::Vector2f leftMostTile = IsometricUtils::tileToScreen(mapEntity.getRows() - 1, 0);
    sf::Vector2f rightMostTile = IsometricUtils::tileToScreen(0, mapEntity.getCols() - 1);
    float mapLeft = leftMostTile.x;
    float mapRight = rightMostTile.x + Tile::TILE_WIDTH;
    float mapTop = IsometricUtils::getMapStartY();
    float mapBottom = IsometricUtils::tileToScreen(mapEntity.getRows() - 1, mapEntity.getCols() - 1).y + Tile::TILE_HEIGHT;
    
    sf::Vector2f viewSize = cameraView.getSize();
    sf::Vector2f viewCenter = cameraView.getCenter();
    float halfWidth = viewSize.x / 2.0f;
    float halfHeight = viewSize.y / 2.0f;

    if (viewCenter.x - halfWidth < mapLeft) {
        viewCenter.x = mapLeft + halfWidth;
    }
    if (viewCenter.x + halfWidth > mapRight) {
        viewCenter.x = mapRight - halfWidth;
    }
    if (viewCenter.y - halfHeight < mapTop) {
        viewCenter.y = mapTop + halfHeight;
    }
    if (viewCenter.y + halfHeight > mapBottom) {
        viewCenter.y = mapBottom - halfHeight;
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

void loadUI(const std::function<void(const std::string&, TileType)>& buildingSelectCallback);