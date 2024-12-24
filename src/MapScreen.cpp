// MapScreen.cpp
#include "MapScreen.hpp"
#include "IsometricUtils.hpp"
#include "TextureManager.hpp"
#include <iostream>

// Constructor: Initializes all components and calls initializeTowers()
MapScreen::MapScreen(int rows, int cols, const sf::Vector2u& windowSize)
    : mapEntity(rows, cols, centralBulletManager), // Initialize Map with central BulletManager
      uiManager(windowSize), 
      tankSpawn(mapEntity),    // Initialize TankSpawn with mapEntity
      skeletonSpawn(mapEntity),
      centralBulletManager() { // Initialize BulletManager
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

    // Set up the camera
    cameraView.setSize(static_cast<float>(windowSize.x), static_cast<float>(windowSize.y));
    sf::Vector2f centerPosition = IsometricUtils::tileToScreen(rows / 2, cols / 2);
    cameraView.setCenter(centerPosition);

    // Load UI elements and set callbacks for building/trap selection
    uiManager.loadUI([this](const std::string& texture, bool isTrap) {
        if (isTrap) {
            setSelectedTrapType(texture);
        } else {
            setSelectedBuildingType(texture);
        }
    });
    // mapEntity.loadTownHallAnimation();
    // Initialize towers
    initializeTowers();
}

// Initializes towers by creating Tower instances and adding them to the towers vector
void MapScreen::initializeTowers() {
    // No towers are initialized by default. Towers should be added via user actions or map loading.
}


// Handles input events and delegates them to respective managers
void MapScreen::handleEvents(const sf::Event& event, sf::RenderWindow& window) {
    uiManager.handleEvent(event);
    tankSpawn.handleEvent(event, mapEntity);
    skeletonSpawn.handleEvent(event, mapEntity);

    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window), cameraView);
        TileCoordinates tileCoords = IsometricUtils::screenToTile(mousePos.x, mousePos.y, mapEntity.getRows(), mapEntity.getCols());
        auto tile = mapEntity.getTile(tileCoords.row, tileCoords.col);
        if (tile) {
            if (!selectedTrapTexture.empty() && !tile->hasTrap() && !tile->getBuilding()) {
                mapEntity.addTrap(tileCoords.row, tileCoords.col, selectedTrapTexture);
            } else if (!selectedBuildingTexture.empty() && !tile->getBuilding() && !tile->hasTrap()) {
                if (selectedBuildingTexture == "../assets/buildings/moontower.png") {
                    // mapEntity.addBuilding(tile->getRow(), tile->getCol(), "../assets/buildings/moontower.png");
                    // Initialize a new tower instance
                    mapEntity.addTower(tile->getRow(), tile->getCol(), selectedBuildingTexture);
                    // sf::Vector2f newTowerPos = IsometricUtils::tileToScreen(tileCoords.row, tileCoords.col);
                    // std::shared_ptr<Tower> newTower = std::make_shared<Tower>((mapEntity.nextTowerId)++, newTowerPos, 200.0f, 1.0f, centralBulletManager, selectedBuildingTexture);
                    // towers.push_back(newTower);
                    // tile->setTower(newTower);
                } else {
                    // mapEntity.addBuilding(tileCoords.row, tileCoords.col, selectedBuildingTexture);
                    mapEntity.addBuilding(tile->getRow(), tile->getCol(), selectedBuildingTexture);
                }
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

// Draws all game elements including background, map, spawns, towers, bullets, and UI
void MapScreen::draw(sf::RenderWindow& window, float deltaTime) {
    window.setView(cameraView);
    window.draw(backgroundSprite);
    mapEntity.draw(window);
    tankSpawn.draw(window, deltaTime, mapEntity);
    skeletonSpawn.draw(window, deltaTime);

    // Draw towers first
    // for (const auto& tower : mapEntity.getTowers()) {
    //     tower->render(window);
    // }

    // Render bullets AFTER towers to ensure they are visible above everything else
    centralBulletManager.render(window);

    // Reset to default view and draw UI
    window.setView(window.getDefaultView());
    uiManager.draw(window);
}

// Sets the selected building type and clears any selected trap type
void MapScreen::setSelectedBuildingType(const std::string& buildingTexture) {
    selectedBuildingTexture = buildingTexture;
    selectedTrapTexture.clear(); // Clear selected trap texture when a building is selected
}

// Sets the selected trap type and clears any selected building type
void MapScreen::setSelectedTrapType(const std::string& trapTexture) {
    selectedTrapTexture = trapTexture;
    selectedBuildingTexture.clear(); // Clear selected building texture when a trap is selected
}

// Moves the camera based on WASD input and clamps it within map boundaries
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

// Saves the current map state to a file
void MapScreen::saveMap(const std::string& filename) {
    mapEntity.saveToFile(filename);
}

// Loads the map state from a file
void MapScreen::loadMap(const std::string& filename) {
    mapEntity.loadFromFile(filename);
}

// Provides access to the map entity
Map& MapScreen::getMapEntity() {
    return mapEntity;
}

// Updates all game logic including spawns, towers, bullets, and handles collisions
void MapScreen::update(float deltaTime) {
    skeletonSpawn.update(deltaTime, mapEntity);
    tankSpawn.update(deltaTime, mapEntity); // Pass mapEntity as the second argument
    centralBulletManager.update(deltaTime);

    // Collect all troop positions
    std::vector<sf::Vector2f> troopPositions;
    for (const auto& skeleton : skeletonSpawn.getSkeletons()) {
        if (skeleton->isAlive()) {
            troopPositions.emplace_back(skeleton->getPosition());
        }
    }
    for (const auto& tank : tankSpawn.getTanks()) {
        if (!tank->isDestroyed()) {
            troopPositions.emplace_back(tank->getPosition());
        }
    }

    // Update all towers with troop positions
    for (auto& tower : mapEntity.getTowers()) {
        tower->update(deltaTime, troopPositions);
    }

    // Handle Bullet-Troop Collisions
    handleBulletCollisions(deltaTime);
}

// Handles collisions between bullets and troops (skeletons and tanks)
void MapScreen::handleBulletCollisions(float deltaTime) {
    for (auto& bullet : centralBulletManager.getBullets()) { // Central BulletManager
        if (!bullet.isActive()) continue;
        sf::FloatRect bulletBounds = bullet.getSprite().getGlobalBounds();
        // Log bullet bounds
        // std::cout << "Bullet bounds: (" << bulletBounds.left << ", "
        //           << bulletBounds.top << ", " << bulletBounds.width << ", "
        //           << bulletBounds.height << ")\n";

        for (auto& skeleton : skeletonSpawn.getSkeletons()) {
            if (!skeleton->isAlive()) continue;
            sf::FloatRect skeletonBounds = skeleton->getSprite().getGlobalBounds();
            // Log skeleton bounds
            // std::cout << "Skeleton bounds: (" << skeletonBounds.left << ", "
            //           << skeletonBounds.top << ", " << skeletonBounds.width << ", "
            //           << skeletonBounds.height << ")\n";
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
            if (tank->isDestroyed()) continue;
            sf::FloatRect tankBounds = tank->getSprite().getGlobalBounds();
            if (bulletBounds.intersects(tankBounds)) {
                std::cout << "Bullet hit Tank at ("
                          << tank->getPosition().x << ", "
                          << tank->getPosition().y << ").\n";
                tank->takeDamage(10, deltaTime); // Apply damage
                bullet.deactivate(); // Deactivate bullet
                break; // Move to next bullet
            }
        }
    }

    // Remove Dead Skeletons and Tanks
    skeletonSpawn.removeDeadSkeletons();
    // tankSpawn.removeDeadTanks();
}