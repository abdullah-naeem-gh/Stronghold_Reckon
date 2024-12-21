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
            sf::Vector2f isoPos = IsometricUtils::tileToScreen(row, col);
            tiles[row][col]->setPosition(isoPos.x, isoPos.y);
        }
    }

    // Add edges between neighboring tiles
    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < cols; ++col) {
            auto tile = tiles[row][col];
            if (row > 0) {
                tile->addEdge(tiles[row - 1][col]); // Up
                if (col > 0) tile->addEdge(tiles[row - 1][col - 1]); // Up-Left
                if (col < cols - 1) tile->addEdge(tiles[row - 1][col + 1]); // Up-Right
            }
            if (row < rows - 1) {
                tile->addEdge(tiles[row + 1][col]); // Down
                if (col > 0) tile->addEdge(tiles[row + 1][col - 1]); // Down-Left
                if (col < cols - 1) tile->addEdge(tiles[row + 1][col + 1]); // Down-Right
            }
            if (col > 0) tile->addEdge(tiles[row][col - 1]); // Left
            if (col < cols - 1) tile->addEdge(tiles[row][col + 1]); // Right
        }
    }
}

std::shared_ptr<Tile> Map::getTile(int row, int col) const {
    if (row < 0 || row >= rows || col < 0 || col >= cols)
        return nullptr;
    return tiles[row][col];
}

bool Map::addBuilding(int row, int col, const std::string& buildingTexture, TileType type) {
    auto tile = getTile(row, col);
    if (!tile) {
        std::cerr << "Invalid tile coordinates: (" << row << ", " << col << ").\n";
        return false;
    }
    if (tile->getBuilding() != nullptr) {
        std::cerr << "Tile already has a building.\n";
        return false;
    }
    sf::Vector2f isoPos = IsometricUtils::tileToScreen(row, col);
    sf::Vector2f buildingPosition = sf::Vector2f(
        isoPos.x + Tile::TILE_WIDTH / 2.0f,
        isoPos.y + Tile::TILE_HEIGHT - (Building::BUILDING_HEIGHT / 2.0f)
    );
    auto building = std::make_shared<Building>(nextBuildingId++, buildingPosition.x, buildingPosition.y, buildingTexture);
    tile->setBuilding(building);
    tile->setType(type); // Set the tile type to the provided type
    saveState();
    return true;
}

bool Map::addWall(int row, int col) {
    auto tile = getTile(row, col);
    if (!tile) {
        std::cerr << "Invalid tile coordinates: (" << row << ", " << col << ").\n";
        return false;
    }
    if (tile->getBuilding() != nullptr) {
        std::cerr << "Tile already has a building.\n";
        return false;
    }
    tile->setType(TileType::Wall); // Set the tile type to Wall
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
                sf::Vector2f isoPos = IsometricUtils::tileToScreen(row, col);
                sf::Vector2f buildingPosition = sf::Vector2f(
                    isoPos.x + Tile::TILE_WIDTH / 2.0f,
                    isoPos.y + Tile::TILE_HEIGHT
                );
                auto building = std::make_shared<Building>(buildingId, buildingPosition.x, buildingPosition.y, texturePath);
                tile->setBuilding(building);
            } else {
                tile->setBuilding(nullptr);
            }
        }
    }
    inFile.close();
    saveState();
}

void Map::saveState() {
    undoStack.push(GameState(tiles));
    while (!redoStack.empty()) {
        redoStack.pop();
    }
}

void Map::undo() {
    if (!undoStack.empty()) {
        redoStack.push(GameState(tiles));
        GameState previousState = undoStack.top();
        undoStack.pop();
        restoreGameState(previousState);
    }
}

void Map::redo() {
    if (!redoStack.empty()) {
        undoStack.push(GameState(tiles));
        GameState nextState = redoStack.top();
        redoStack.pop();
        restoreGameState(nextState);
    }
}

void Map::restoreGameState(const GameState& state) {
    tiles = state.getTiles();
    for (const auto& row : tiles) {
        for (const auto& tile : row) {
            if (tile->getType() == TileType::Building && tile->getBuilding()) {
                sf::Vector2f tilePos = tile->getPosition();
                sf::Vector2f buildingPosition = sf::Vector2f(
                    tilePos.x + Tile::TILE_WIDTH / 2.0f,
                    tilePos.y + Tile::TILE_HEIGHT
                );
                tile->getBuilding()->setPosition(buildingPosition.x, buildingPosition.y);
                if (tile->getBuilding()->getId() >= nextBuildingId) {
                    nextBuildingId = tile->getBuilding()->getId() + 1;
                }
            }
        }
    }
}

std::vector<std::vector<std::shared_ptr<Tile>>> Map::getTiles() {
    return tiles;
}

std::vector<std::shared_ptr<Tile>> Map::getNeighbors(std::shared_ptr<Tile>& tile) const {
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