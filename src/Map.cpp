#include "Map.hpp"
#include "IsometricUtils.hpp"
#include <iostream>

Map::Map(int rows, int cols)
    : rows(rows), cols(cols), nextBuildingId(1) {
    initializeTiles();
}

void Map::initializeTiles() {
    tiles.resize(rows, std::vector<std::shared_ptr<Tile>>(cols, nullptr));
    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < cols; ++col) {
            tiles[row][col] = std::make_shared<Tile>(row, col);
            sf::Vector2f isoPos = IsometricUtils::tileToScreen(row, col, START_X, START_Y);
            tiles[row][col]->setPosition(isoPos.x, isoPos.y);
        }
    }
}

std::shared_ptr<Tile> Map::getTile(int row, int col) const {
    if (row < 0 || row >= rows || col < 0 || col >= cols)
        return nullptr;
    return tiles[row][col];
}

std::shared_ptr<Building> Map::addBuilding(int row, int col, const std::string& buildingTexture) {
    auto tile = getTile(row, col);
    if (!tile) {
        std::cerr << "Invalid tile coordinates: (" << row << ", " << col << ").\n";
        return nullptr;
    }
    if (tile->getBuilding() != nullptr) {
        std::cerr << "Tile already has a building.\n";
        return nullptr;
    }

    sf::Vector2f isoPos = IsometricUtils::tileToScreen(row, col, START_X, START_Y);
    sf::Vector2f buildingPosition = sf::Vector2f(
        isoPos.x + Tile::TILE_WIDTH / 2.0f - Building::BUILDING_WIDTH / 2.0f,
        isoPos.y + Tile::TILE_HEIGHT - Building::BUILDING_HEIGHT / 2.0f
    );

    // Place the building and save the current state
    auto building = std::make_shared<Building>(nextBuildingId++, buildingPosition.x, buildingPosition.y, buildingTexture);
    buildings.push_back(building);
    tile->setBuilding(building);

    saveState();
    std::cout << "Building added and state saved. Undo stack size: " << undoStack.size() << std::endl;

    return building;
}

void Map::draw(sf::RenderWindow& window) const {
    for (const auto& row : tiles) {
        for (const auto& tile : row) {
            tile->draw(window);
        }
    }
}

int Map::getRows() const {
    return rows;
}

int Map::getCols() const {
    return cols;
}

void Map::saveToFile(const std::string& filename) {
    std::ofstream outFile(filename);
    if (!outFile) {
        std::cerr << "Error opening file for writing.\n";
        return;
    }
    outFile << rows << " " << cols << "\n";
    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < cols; ++col) {
            auto tile = getTile(row, col);
            outFile << static_cast<int>(tile->getType()) << " ";
            auto building = tile->getBuilding();
            if (building) {
                outFile << building->getId() << " " << building->getTexturePath() << " ";
            } else {
                outFile << "-1 - ";
            }
            outFile << "\n";
        }
    }
    outFile.close();
}

void Map::loadFromFile(const std::string& filename) {
    std::ifstream inFile(filename);
    if (!inFile) {
        std::cerr << "Error opening file for reading.\n";
        return;
    }
    inFile >> rows >> cols;
    tiles.clear();
    initializeTiles();
    int tileType, buildingId;
    std::string texturePath;
    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < cols; ++col) {
            inFile >> tileType >> buildingId >> texturePath;
            auto tile = getTile(row, col);
            tile->setType(static_cast<TileType>(tileType));
            if (buildingId != -1) {
                auto building = std::make_shared<Building>(buildingId, 0, 0, texturePath);
                building->setPosition(tile->getPosition().x, tile->getPosition().y);
                tile->setBuilding(building);
            } else {
                tile->setBuilding(nullptr);
            }
        }
    }
    inFile.close();
    
    // Save the state of the just-loaded map
    saveState();
    std::cout << "Map loaded and state saved. Undo stack size: " << undoStack.size() << std::endl;
}

void Map::saveState() {
    // Create a new GameState object representing the current state of tiles and buildings
    undoStack.push(GameState(tiles, buildings));
    while (!redoStack.empty()) {
        redoStack.pop(); // Clear redo stack as forward actions are invalidated by new state creation
    }
    std::cout << "State saved. Undo stack size: " << undoStack.size() << std::endl;
}

void Map::undo() {
    if (!undoStack.empty()) {
        redoStack.push(GameState(tiles, buildings));
        GameState previousState = undoStack.top();
        undoStack.pop();
        restoreGameState(previousState);
        std::cout << "Undo performed. Undo stack size: " << undoStack.size()
                  << ", Redo stack size: " << redoStack.size() << std::endl;
    } else {
        std::cout << "Undo stack is empty, cannot perform undo." << std::endl;
    }
}

void Map::redo() {
    if (!redoStack.empty()) {
        undoStack.push(GameState(tiles, buildings));
        GameState nextState = redoStack.top();
        redoStack.pop();
        restoreGameState(nextState);
        std::cout << "Redo performed. Undo stack size: " << undoStack.size()
                  << ", Redo stack size: " << redoStack.size() << std::endl;
    } else {
        std::cout << "Redo stack is empty, cannot perform redo." << std::endl;
    }
}

void Map::restoreGameState(const GameState& state) {
    tiles = state.getTiles();
    buildings = state.getBuildings();

    // Re-associate buildings with their tiles
    for (const auto& row : tiles) {
        for (const auto& tile : row) {
            tile->setBuilding(nullptr); // Clear existing building references
        }
    }

    for (auto& building : buildings) {
        sf::Vector2f buildingPos = building->getPosition();
        TileCoordinates coords = IsometricUtils::screenToTile(buildingPos.x, buildingPos.y, START_X, START_Y);
        auto tile = getTile(coords.row, coords.col);
        if (tile) {
            tile->setBuilding(building);
        }
    }

    // Refresh all tiles' textures based on current tile types and associations
    for (const auto& row : tiles) {
        for (const auto& tile : row) {
            if (tile) {
                tile->updateTexture();
            }
        }
    }
}