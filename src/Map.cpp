// Map.cpp
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

bool Map::addBuilding(int row, int col, const std::string& buildingTexture) {
    auto tile = getTile(row, col);
    if (!tile) {
        std::cerr << "Invalid tile coordinates: (" << row << ", " << col << ").\n";
        return false;
    }
    if (tile->getBuilding() != nullptr) {
        std::cerr << "Tile already has a building.\n";
        return false;
    }
    sf::Vector2f isoPos = IsometricUtils::tileToScreen(row, col, START_X, START_Y);
    sf::Vector2f buildingPosition = sf::Vector2f(
        isoPos.x + Tile::TILE_WIDTH / 2.0f, // Center X of the tile
        isoPos.y + Tile::TILE_HEIGHT        // Bottom Y of the tile
    );

    // Create the building
    auto building = std::make_shared<Building>(nextBuildingId++, buildingPosition.x, buildingPosition.y, buildingTexture);
    tile->setBuilding(building);

    std::cout << "Building placed at tile: (" << row << ", " << col << ").\n";
    saveState();
    return true;
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
                outFile << building->getId() << " " << building->getTexturePath() << "\n";
            } else {
                outFile << "-1 -\n";
            }
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
                sf::Vector2f isoPos = IsometricUtils::tileToScreen(row, col, START_X, START_Y);
                sf::Vector2f buildingPosition = sf::Vector2f(
                    isoPos.x + Tile::TILE_WIDTH / 2.0f, // Center X
                    isoPos.y + Tile::TILE_HEIGHT        // Bottom Y
                );
                auto building = std::make_shared<Building>(buildingId, buildingPosition.x, buildingPosition.y, texturePath);
                tile->setBuilding(building);
            }
            else {
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
    // Create a new GameState object representing the current state of tiles
    undoStack.push(GameState(tiles));
    while (!redoStack.empty()) {
        redoStack.pop(); // Clear redo stack as forward actions are invalidated by new state creation
    }
    std::cout << "State saved. Undo stack size: " << undoStack.size() << std::endl;
}

void Map::undo() {
    if (!undoStack.empty()) {
        // Save current state to redo stack
        redoStack.push(GameState(tiles));
        // Restore the last state
        GameState previousState = undoStack.top();
        undoStack.pop();
        restoreGameState(previousState);
        std::cout << "Undo performed. Undo stack size: " << undoStack.size()
                  << ", Redo stack size: " << redoStack.size() << std::endl;
    }
    else {
        std::cout << "Undo stack is empty, cannot perform undo." << std::endl;
    }
}

void Map::redo() {
    if (!redoStack.empty()) {
        // Save current state to undo stack
        undoStack.push(GameState(tiles));
        // Restore the next state
        GameState nextState = redoStack.top();
        redoStack.pop();
        restoreGameState(nextState);
        std::cout << "Redo performed. Undo stack size: " << undoStack.size()
                  << ", Redo stack size: " << redoStack.size() << std::endl;
    }
    else {
        std::cout << "Redo stack is empty, cannot perform redo." << std::endl;
    }
}

void Map::restoreGameState(const GameState& state) {
    tiles = state.getTiles();

    // Re-position buildings based on their associated tiles
    for (const auto& row : tiles) {
        for (const auto& tile : row) {
            if (tile->getType() == TileType::Building && tile->getBuilding()) {
                // Obtain tile position
                sf::Vector2f tilePos = tile->getPosition();
                sf::Vector2f buildingPosition = sf::Vector2f(
                    tilePos.x + Tile::TILE_WIDTH / 2.0f, // Center X
                    tilePos.y + Tile::TILE_HEIGHT        // Bottom Y
                );
                tile->getBuilding()->setPosition(buildingPosition.x, buildingPosition.y);

                // Update nextBuildingId if necessary
                if (tile->getBuilding()->getId() >= nextBuildingId) {
                    nextBuildingId = tile->getBuilding()->getId() + 1;
                }
            }
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

    std::cout << "GameState restored. Undo stack size: " << undoStack.size() << std::endl;
}

std::vector<std::vector<std::shared_ptr<Tile>>> Map::getTiles() {
    return tiles;
}

std::vector<std::shared_ptr<Tile>> Map::getNeighbors(std::shared_ptr<Tile>& tile) const{
    std::cout << "get neigh\n";
    std::vector<std::shared_ptr<Tile>> neighbors;
    
    int row = tile->getRow();
    int col = tile->getCol();
    std::cout << "Tile coord: (" << tile->getPosition().x << ", " << tile->getPosition().y << ")\n";
    std::cout << "Tile Position: (" << row << ", " << col << ")\n";

    if (row > 0 && !this->tiles[row - 1][col]->isBlocked()) 
        neighbors.push_back(this->tiles[row - 1][col]); // Up
    if (row < tiles.size() - 1 && !tiles[row + 1][col]->isBlocked()) 
        neighbors.push_back(tiles[row + 1][col]); // Down
    if (col > 0 && !tiles[row][col - 1]->isBlocked()) 
        neighbors.push_back(tiles[row][col - 1]); // Left
    if (col < tiles[row].size() - 1 && !tiles[row][col + 1]->isBlocked()) 
        neighbors.push_back(tiles[row][col + 1]); // Right

    return neighbors;
}