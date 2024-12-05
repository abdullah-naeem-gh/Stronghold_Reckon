#include "Map.hpp"
#include "IsometricUtils.hpp"
#include <iostream>
#include <fstream>
#include "TextureManager.hpp"


// Constructor that initializes the map with the given number of rows and columns
Map::Map(int rows, int cols)
    : rows(rows), cols(cols), nextBuildingId(1) {
    initializeTiles();
}


// Initializes the tiles for the map
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

// Retrieves a tile at the specified row and column
std::shared_ptr<Tile> Map::getTile(int row, int col) const {
    if (row < 0 || row >= rows || col < 0 || col >= cols)
        return nullptr;
    return tiles[row][col];
}

// Adds a building to the specified tile position
std::shared_ptr<Building> Map::addBuilding(int row, int col, const std::string& buildingTexture) {
    auto tile = getTile(row, col);
    if (!tile) {
        std::cerr << "Invalid tile coordinates: (" << row << ", " << col << ").\n";
        return nullptr;
    }
    sf::Vector2f isoPos = IsometricUtils::tileToScreen(row, col, START_X, START_Y);
    sf::Vector2f buildingPosition = sf::Vector2f(
        isoPos.x + Tile::TILE_WIDTH / 2.0f - Building::BUILDING_WIDTH / 2.0f,
        isoPos.y + Tile::TILE_HEIGHT - Building::BUILDING_HEIGHT / 2.0f
    );
    auto building = std::make_shared<Building>(nextBuildingId++, isoPos.x, isoPos.y, buildingTexture);
    building->setPosition(buildingPosition.x, buildingPosition.y);
    buildings.push_back(building);
    tile->setBuilding(building);
    return building;
}

// Saves the current state of the map to a file
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
            if (tile) {
                outFile << static_cast<int>(tile->getType()) << " ";
                auto building = tile->getBuilding();
                if (building) {
                    outFile << building->getId() << " " << building->getTexturePath() << " ";
                } else {
                    outFile << "-1 - ";  // Indicate no building
                }
            }
            outFile << "\n";
        }
    }
    outFile.close();
}

// Loads the map state from a file
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
            }
        }
    }
    inFile.close();
}

// Saves the current state for undo functionality
void Map::saveState() {
    undoStack.push(GameState(tiles, buildings));
    while (!redoStack.empty()) redoStack.pop(); // Clear redo stack as forward actions are invalidated
    std::cout << "State saved. Undo stack size: " << undoStack.size() << std::endl;
}

void Map::undo() {
    if (!undoStack.empty()) {
        redoStack.push(GameState(tiles, buildings));
        GameState previousState = undoStack.top();
        undoStack.pop();
        restoreGameState(previousState);
        std::cout << "Undo performed. Undo stack size: " << undoStack.size() << ", Redo stack size: " << redoStack.size() << std::endl;
    }
}

void Map::redo() {
    if (!redoStack.empty()) {
        undoStack.push(GameState(tiles, buildings));
        GameState nextState = redoStack.top();
        redoStack.pop();
        restoreGameState(nextState);
        std::cout << "Redo performed. Undo stack size: " << undoStack.size() << ", Redo stack size: " << redoStack.size() << std::endl;
    }
}

void Map::restoreGameState(const GameState& state) {
    tiles = state.getTiles();
    buildings = state.getBuildings();

    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < cols; ++col) {
            // Restore each tile's type and associated building
            auto tile = tiles[row][col];
            if (tile) {
                tile->setBuilding(nullptr); // Clear existing building references
                tile->updateTexture();
                
                for (const auto& building : buildings) {
                    // Match building positions with their respective tiles and reconstruct links
                    sf::Vector2f buildingPos = building->getPosition();
                    TileCoordinates coords = IsometricUtils::screenToTile(buildingPos.x, buildingPos.y, START_X, START_Y);

                    if (coords.row == row && coords.col == col) {
                        tile->setBuilding(building);
                        break;
                    }
                }
            }
        }
    }
}

// Draws the tiles and buildings on the map
void Map::draw(sf::RenderWindow& window) const {
    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < cols; ++col) {
            tiles[row][col]->draw(window);
        }
    }
}

// Getter for the number of rows
int Map::getRows() const {
    return rows;
}

// Getter for the number of columns
int Map::getCols() const {
    return cols;
}

std::vector<std::shared_ptr<Tile>> Map::getNeighbors(std::shared_ptr<Tile> tile) const {
    std::vector<std::shared_ptr<Tile>> neighbors;
    int directions[4][2] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}}; // Directions: down, up, right, left

    for (auto& dir : directions) {
        int newRow = tile->getRow() + dir[0];
        int newCol = tile->getCol() + dir[1];

        if (newRow >= 0 && newRow < rows && newCol >= 0 && newCol < cols) {
            auto neighbor = getTile(newRow, newCol);
            if (neighbor && neighbor->isWalkable()) {
                neighbors.push_back(neighbor);
            }
        }
    }
    return neighbors;
}
