#include "MapScreen.hpp"
#include "IsometricUtils.hpp"
#include <iostream>

MapScreen::MapScreen(int rows, int cols, const sf::Vector2u& windowSize)
    : mapEntity(rows, cols),
      uiManager(windowSize),
      tankSpawn(mapEntity),
      skeletonSpawn(mapEntity),
      centralBulletManager()  { // Central BulletManager
    // Load the background texture
    if (!backgroundTexture.loadFromFile("../assets/background/map_bg.png")) {
        std::cerr << "Error loading background image" << std::endl;
    }
    backgroundSprite.setTexture(backgroundTexture);
    backgroundSprite.setScale(
        1920.0f / static_cast<float>(backgroundTexture.getSize().x),
        960.0f / static_cast<float>(backgroundTexture.getSize().y)
    );
    backgroundSprite.setPosition(-528, 50);
    cameraView.setSize(static_cast<float>(windowSize.x), static_cast<float>(windowSize.y));
    sf::Vector2f centerPosition = IsometricUtils::tileToScreen(rows / 2, cols / 2);
    cameraView.setCenter(centerPosition);
    uiManager.loadUI([this](const std::string& buildingTexture) {
        setSelectedBuildingType(buildingTexture);
    });
    initializeTowers(); // Initialize towers and pass central BulletManager
}

void MapScreen::initializeTowers() {
    // Example: Initialize a tower at tile (10, 10)
    sf::Vector2f towerPosition = IsometricUtils::tileToScreen(10, 10);
    std::shared_ptr<Tower> tower = std::make_shared<Tower>(towerPosition, 200.0f, 1.0f, centralBulletManager);
    towers.push_back(tower);
    // Add more towers as needed, e.g.,
    // sf::Vector2f anotherTowerPos = IsometricUtils::tileToScreen(12, 12);
    // std::shared_ptr<Tower> anotherTower = std::make_shared<Tower>(anotherTowerPos, 200.0f, 1.0f, centralBulletManager);
    // towers.push_back(anotherTower);
}


void MapScreen::handleEvents(const sf::Event& event, sf::RenderWindow& window) {
    uiManager.handleEvent(event);
    tankSpawn.handleEvent(event, mapEntity);
    skeletonSpawn.handleEvent(event, mapEntity);

    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window), cameraView);
        TileCoordinates tileCoords = IsometricUtils::screenToTile(mousePos.x, mousePos.y, mapEntity.getRows(), mapEntity.getCols());
        auto tile = mapEntity.getTile(tileCoords.row, tileCoords.col);
        if (tile && !selectedBuildingTexture.empty()) {
            // Corrected condition to match the actual tower texture path
            if (selectedBuildingTexture == "../assets/buildings/moontower.png") {
                mapEntity.addBuilding(tileCoords.row, tileCoords.col, "../assets/buildings/moontower.png");
                
                // Initialize a new tower instance
                sf::Vector2f newTowerPos = IsometricUtils::tileToScreen(tileCoords.row, tileCoords.col);
                std::shared_ptr<Tower> newTower = std::make_shared<Tower>(newTowerPos, 200.0f, 1.0f, centralBulletManager);
                towers.push_back(newTower);
            }
            else if (!tile->getBuilding()) {
                mapEntity.addBuilding(tileCoords.row, tileCoords.col, selectedBuildingTexture);
            }
        }
    }

    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::B) {
            sf::Vector2f startTilePos = IsometricUtils::tileToScreen(14, 14);
            sf::Vector2f targetTilePos = IsometricUtils::tileToScreen(1, 28);
            float bulletSpeed = 300.0f;
            centralBulletManager.fireBullet(startTilePos, targetTilePos, bulletSpeed);
        }
    }
}

void MapScreen::draw(sf::RenderWindow& window, float deltaTime) {
    window.setView(cameraView);
    window.draw(backgroundSprite);
    mapEntity.draw(window);
    tankSpawn.draw(window, deltaTime, mapEntity);
    skeletonSpawn.draw(window, deltaTime);
    // Draw towers first
    for (const auto& tower : towers) {
        tower->render(window);
    }
    // Render bullets AFTER towers to ensure they are visible above everything else
    centralBulletManager.render(window);
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
    // Clamp camera within map boundaries
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

void MapScreen::update(float deltaTime) {
    skeletonSpawn.update(deltaTime, mapEntity);
    tankSpawn.update(deltaTime, mapEntity);
    centralBulletManager.update(deltaTime);
    // Collect all troop positions
    std::vector<sf::Vector2f> troopPositions;
    for (const auto& skeleton : skeletonSpawn.getSkeletons()) {
        if (skeleton->isAlive()) {
            troopPositions.emplace_back(skeleton->getPosition());
        }
    }
    for (const auto& tank : tankSpawn.getTanks()) {
        if (tank.isAlive()) {
            troopPositions.emplace_back(tank.getPosition());
        }
    }
    // Update all towers with troop positions
    for (auto& tower : towers) {
        tower->update(deltaTime, troopPositions);
    }
    // Handle Bullet-Troop Collisions
    handleBulletCollisions();
}

void MapScreen::handleBulletCollisions() {
    for (auto& bullet : centralBulletManager.getBullets()) { // Central BulletManager
        if (!bullet.isActive()) continue;
        sf::FloatRect bulletBounds = bullet.getSprite().getGlobalBounds();

        // Log bullet bounds
        std::cout << "Bullet bounds: (" << bulletBounds.left << ", "
                  << bulletBounds.top << ", " << bulletBounds.width << ", "
                  << bulletBounds.height << ")\n";

        for (auto& skeleton : skeletonSpawn.getSkeletons()) {
            if (!skeleton->isAlive()) continue;
            sf::FloatRect skeletonBounds = skeleton->getSprite().getGlobalBounds();

            // Log skeleton bounds
            std::cout << "Skeleton bounds: (" << skeletonBounds.left << ", "
                      << skeletonBounds.top << ", " << skeletonBounds.width << ", "
                      << skeletonBounds.height << ")\n";

            if (bulletBounds.intersects(skeletonBounds)) {
                std::cout << "Bullet hit Skeleton at ("
                          << skeleton->getPosition().x << ", "
                          << skeleton->getPosition().y << ").\n";
                skeleton->takeDamage(10); // Apply damage
                bullet.deactivate(); // Deactivate bullet
                break; // Move to next bullet
            }
        }

        for (auto& tank : tankSpawn.getTanks()) {
            if (!tank.isAlive()) continue;
            sf::FloatRect tankBounds = tank.getSprite().getGlobalBounds();

            if (bulletBounds.intersects(tankBounds)) {
                std::cout << "Bullet hit Tank at ("
                          << tank.getPosition().x << ", "
                          << tank.getPosition().y << ").\n";
                tank.takeDamage(10); // Apply damage
                bullet.deactivate(); // Deactivate bullet
                break; // Move to next bullet
            }
        }
    }

    // Remove Dead Skeletons and Tanks
    skeletonSpawn.removeDeadSkeletons();
    tankSpawn.removeDeadTanks();
}