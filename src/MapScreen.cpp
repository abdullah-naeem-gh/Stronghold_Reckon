#include "MapScreen.hpp"
#include <iostream>
#include <algorithm> // For std::reverse
#include <queue>     // For std::priority_queue
#include <unordered_map> // For std::unordered_map

MapScreen::MapScreen(int rows, int cols, const sf::Vector2u& windowSize)
    : mapEntity(rows, cols), uiManager(windowSize), player(0.0f, 0.0f) {
    
    cameraView.setSize(static_cast<float>(windowSize.x), static_cast<float>(windowSize.y));
    int centerRow = mapEntity.getRows() / 2;
    int centerCol = mapEntity.getCols() / 2;
    sf::Vector2f centerPosition = IsometricUtils::tileToScreen(centerRow, centerCol, 400.0f, 50.0f);
    cameraView.setCenter(centerPosition);
    uiManager.loadUI([this](const std::string& buildingTexture) {
        setSelectedBuildingType(buildingTexture);
    });

    initializePath(); // Initialize path after player setup
    std::cout << "MapScreen initialized successfully." << std::endl;
}

void MapScreen::initializePath() {
    std::shared_ptr<Tile> startTile = mapEntity.getTile(playerInitialRow, 0);
    std::shared_ptr<Tile> goalTile = findTownhallTile();

    if (startTile && goalTile) {
        std::vector<std::shared_ptr<Tile>> path = pathfind(startTile, goalTile, mapEntity);
        player.setPath(path);
    }
}

void MapScreen::handleEvents(const sf::Event& event, sf::RenderWindow& window) {
    uiManager.handleEvent(event);
    player.updateMovement(1.0f); // Assuming consistent movement with fixed delta time
    
    mapEntity.draw(window);
    player.draw(window);
    uiManager.draw(window);
}

void MapScreen::draw(sf::RenderWindow& window) {
    window.setView(cameraView);
    mapEntity.draw(window);
    player.updateMovement(1.0f / 60.0f); // Move the player
    player.draw(window);
    window.setView(window.getDefaultView());
    uiManager.draw(window);
}

void MapScreen::setSelectedBuildingType(const std::string& buildingTexture) {
    selectedBuildingTexture = buildingTexture;
    std::cout << "Selected Building Texture: " << buildingTexture << std::endl;
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

    sf::Vector2f viewSize = cameraView.getSize();
    sf::Vector2f viewCenter = cameraView.getCenter();
    int totalRows = mapEntity.getRows();
    int totalCols = mapEntity.getCols();

    sf::Vector2f furthestTile = IsometricUtils::tileToScreen(totalRows - 1, totalCols - 1, 400.0f, 50.0f);
    float mapWidth = furthestTile.x + Tile::TILE_WIDTH;
    float mapHeight = furthestTile.y + Tile::TILE_HEIGHT;

    float halfWidth = viewSize.x / 2.0f;
    float halfHeight = viewSize.y / 2.0f;

    if (viewCenter.x - halfWidth < 0) {
        viewCenter.x = halfWidth;
    }
    if (viewCenter.x + halfWidth > mapWidth) {
        viewCenter.x = mapWidth - halfWidth;
    }
    if (viewCenter.y - halfHeight < 0) {
        viewCenter.y = halfHeight;
    }
    if (viewCenter.y + halfHeight > mapHeight) {
        viewCenter.y = mapHeight - halfHeight;
    }

    cameraView.setCenter(viewCenter);
}

std::shared_ptr<Tile> MapScreen::findTownhallTile() {
    for (int row = 0; row < mapEntity.getRows(); ++row) {
        for (int col = 0; col < mapEntity.getCols(); ++col) {
            auto tile = mapEntity.getTile(row, col);
            if (tile->getBuilding() && tile->getBuilding()->getTexturePath() == "../assets/buildings/townhall.png") {
                return tile;
            }
        }
    }
    return nullptr;
}

std::vector<std::shared_ptr<Tile>> MapScreen::pathfind(std::shared_ptr<Tile> start, std::shared_ptr<Tile> goal, const Map& map) {
    using PathNode = std::pair<float, std::shared_ptr<Tile>>; // (priority, tile)
    std::priority_queue<PathNode, std::vector<PathNode>, std::greater<PathNode>> openSet; // Min-heap
    std::unordered_map<std::shared_ptr<Tile>, std::shared_ptr<Tile>> cameFrom;
    std::unordered_map<std::shared_ptr<Tile>, float> gScore;
    gScore[start] = 0.0f;
    
    auto heuristic = [](std::shared_ptr<Tile> a, std::shared_ptr<Tile> b) {
        int dx = abs(a->getRow() - b->getRow());
        int dy = abs(a->getCol() - b->getCol());
        return static_cast<float>(dx + dy); // Manhattan distance
    };
    
    openSet.emplace(0.0f, start);
    
    while (!openSet.empty()) {
        auto current = openSet.top().second;
        if (current == goal) {
            std::vector<std::shared_ptr<Tile>> path;
            for (auto tile = goal; tile != nullptr; tile = cameFrom[tile]) {
                path.push_back(tile);
            }
            std::reverse(path.begin(), path.end());
            return path;
        }
        
        openSet.pop();
        
        for (auto& neighbor : map.getNeighbors(current)) {
            float tentativeGScore = gScore[current] + 1.0f; // Assuming uniform cost for simplicity
            if (tentativeGScore < gScore[neighbor] || gScore.find(neighbor) == gScore.end()) {
                cameFrom[neighbor] = current;
                gScore[neighbor] = tentativeGScore;
                float fScore = tentativeGScore + heuristic(neighbor, goal);
                openSet.emplace(fScore, neighbor);
            }
        }
    }
    
    return {}; // No path found
}

void MapScreen::saveMap(const std::string &filename) {
    mapEntity.saveToFile(filename);
}

void MapScreen::loadMap(const std::string &filename) {
    mapEntity.saveState();
    mapEntity.loadFromFile(filename);
    mapEntity.saveState();
}
