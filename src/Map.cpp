// Map.cpp
#include "Map.hpp"
#include "IsometricUtils.hpp"
#include <iostream>
#include <optional>
#include <queue>
#include <tuple>
#include <set>
#include <random>

// Hardcoded Grass Tile Index Map from your generated output
static const std::vector<std::vector<int>> grassTileIndexMap = {
    { 12, 1, 10, 17, 7, 16, 9, 10, 12, 7, 14, 0, 14, 6, 14, 1, 13, 10, 0, 16, 1, 8, 8, 3, 16, 5, 1, 7, 8, 4 },
    { 10, 9, 12, 17, 3, 9, 1, 1, 1, 6, 14, 4, 13, 8, 6, 16, 2, 12, 15, 4, 12, 17, 6, 16, 6, 17, 10, 7, 5, 8 },
    { 13, 2, 4, 12, 8, 1, 9, 6, 1, 1, 13, 14, 9, 10, 7, 14, 13, 14, 13, 13, 12, 14, 14, 5, 17, 13, 7, 11, 10, 10 },
    { 2, 9, 8, 12, 5, 8, 12, 1, 13, 10, 10, 8, 14, 5, 17, 4, 2, 6, 2, 10, 4, 13, 4, 3, 15, 10, 10, 0, 10, 3 },
    { 8, 17, 16, 15, 9, 6, 1, 13, 10, 3, 0, 9, 11, 5, 2, 1, 10, 2, 3, 4, 6, 4, 4, 13, 0, 11, 6, 10, 5, 12 },
    { 10, 16, 6, 13, 5, 8, 15, 2, 12, 7, 9, 16, 13, 13, 7, 10, 16, 3, 14, 8, 11, 10, 3, 8, 2, 14, 11, 1, 11, 8 },
    { 4, 11, 11, 7, 16, 7, 3, 9, 0, 7, 10, 3, 6, 7, 5, 2, 1, 10, 11, 6, 14, 4, 16, 12, 1, 16, 2, 10, 14, 3 },
    { 6, 16, 5, 8, 15, 13, 8, 17, 6, 8, 14, 16, 3, 16, 1, 12, 7, 1, 10, 11, 8, 8, 1, 10, 12, 3, 8, 7, 3, 14 },
    { 9, 4, 14, 11, 11, 11, 0, 4, 5, 2, 12, 7, 0, 13, 0, 16, 4, 6, 3, 5, 15, 8, 10, 2, 3, 0, 9, 3, 4, 0 },
    { 14, 4, 15, 10, 14, 4, 10, 12, 2, 9, 9, 16, 16, 17, 10, 6, 8, 17, 0, 12, 13, 10, 6, 17, 11, 14, 7, 14, 10, 6 },
    { 1, 7, 9, 17, 17, 6, 1, 0, 6, 7, 12, 1, 13, 14, 2, 15, 11, 1, 10, 4, 4, 3, 4, 3, 11, 6, 9, 8, 2, 11 },
    { 12, 17, 11, 11, 13, 8, 6, 4, 2, 11, 11, 2, 14, 2, 1, 13, 0, 17, 12, 16, 14, 1, 16, 9, 7, 2, 4, 1, 2, 6 },
    { 11, 8, 9, 14, 1, 0, 8, 17, 14, 4, 16, 11, 5, 13, 16, 5, 5, 3, 5, 1, 11, 10, 4, 5, 7, 3, 12, 16, 15, 14 },
    { 12, 12, 15, 16, 2, 15, 11, 4, 16, 16, 2, 6, 17, 14, 5, 3, 14, 3, 4, 11, 6, 9, 13, 0, 8, 1, 11, 8, 6, 15 },
    { 2, 1, 2, 0, 10, 11, 15, 4, 0, 7, 2, 3, 0, 8, 17, 1, 15, 13, 6, 12, 0, 4, 15, 17, 8, 4, 2, 1, 16, 7 },
    { 16, 6, 17, 16, 14, 16, 5, 13, 3, 7, 16, 16, 9, 13, 16, 7, 14, 11, 17, 14, 3, 3, 16, 7, 4, 15, 17, 5, 2, 14 },
    { 8, 6, 13, 16, 4, 3, 14, 4, 9, 0, 13, 1, 13, 16, 10, 3, 4, 1, 16, 0, 16, 11, 3, 14, 7, 14, 13, 1, 4, 17 },
    { 2, 5, 6, 7, 9, 4, 12, 7, 6, 3, 4, 3, 2, 0, 13, 9, 3, 2, 0, 1, 5, 2, 12, 2, 15, 13, 17, 12, 6, 13 },
    { 7, 10, 13, 15, 14, 7, 8, 15, 10, 7, 10, 4, 3, 4, 13, 4, 9, 12, 8, 5, 16, 9, 3, 10, 5, 2, 7, 3, 2, 12 },
    { 9, 16, 16, 0, 7, 8, 3, 1, 5, 1, 3, 11, 12, 3, 15, 4, 15, 11, 4, 11, 9, 11, 0, 5, 16, 1, 6, 13, 11, 3 },
    { 1, 9, 12, 0, 15, 3, 8, 9, 11, 15, 9, 3, 9, 6, 14, 1, 13, 3, 10, 17, 9, 17, 7, 7, 2, 11, 5, 4, 16, 11 },
    { 11, 6, 1, 14, 14, 3, 7, 9, 12, 9, 12, 6, 10, 16, 7, 1, 6, 11, 3, 3, 12, 12, 10, 16, 14, 4, 0, 1, 1, 15 },
    { 2, 10, 0, 7, 3, 3, 0, 5, 17, 1, 3, 13, 14, 2, 1, 13, 5, 14, 5, 6, 8, 10, 5, 17, 3, 12, 3, 3, 7, 0 },
    { 10, 3, 10, 13, 9, 5, 12, 17, 15, 14, 0, 15, 12, 5, 15, 10, 3, 17, 2, 0, 16, 17, 3, 17, 16, 6, 10, 10, 13, 6 },
    { 6, 14, 9, 14, 15, 2, 13, 13, 10, 12, 6, 16, 7, 16, 4, 7, 7, 9, 10, 2, 11, 0, 11, 7, 10, 5, 11, 8, 1, 7 },
    { 7, 1, 11, 0, 12, 17, 4, 5, 4, 8, 6, 2, 7, 14, 8, 8, 3, 3, 2, 17, 1, 2, 15, 5, 1, 2, 6, 4, 8, 0 },
    { 15, 16, 3, 7, 3, 5, 14, 12, 1, 2, 13, 1, 0, 7, 9, 2, 12, 12, 12, 0, 6, 6, 0, 3, 3, 0, 11, 8, 14, 14 },
    { 14, 9, 11, 4, 9, 9, 17, 1, 13, 11, 14, 15, 3, 8, 16, 0, 16, 14, 15, 6, 11, 1, 13, 15, 3, 7, 9, 11, 16, 11 },
    { 10, 13, 0, 11, 2, 3, 7, 16, 8, 14, 15, 6, 12, 6, 13, 2, 10, 16, 9, 8, 2, 8, 12, 0, 11, 10, 14, 13, 13, 16 },
    { 10, 4, 2, 9, 13, 7, 10, 3, 0, 6, 7, 10, 13, 0, 4, 17, 10, 11, 11, 15, 6, 4, 12, 2, 8, 5, 3, 9, 14, 13 }
};

Map::Map(int rows, int cols)
    : rows(rows), cols(cols), nextBuildingId(1) {
    initializeTiles();
    saveState(); // Save the initial state
}

void Map::initializeTiles() {
    if (rows != grassTileIndexMap.size() || (rows > 0 && cols != grassTileIndexMap[0].size())) {
        throw std::runtime_error("Map dimensions do not match hardcoded grassTileIndexMap dimensions.");
    }

    tiles.resize(rows, std::vector<std::shared_ptr<Tile>>(cols, nullptr));
    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < cols; ++col) {
            // Initialize grass tiles with the specific index
            tiles[row][col] = std::make_shared<Tile>(row, col, TileType::Grass);
            tiles[row][col]->setGrassTileIndex(grassTileIndexMap[row][col]); // Set the specific grass texture
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

bool Map::addBuilding(int row, int col, const std::string& buildingTexture) {
    auto tile = getTile(row, col);
    if (!tile) {
        std::cerr << "Invalid tile coordinates: (" << row << ", " << col << ").\n";
        return false;
    }
    if (tile->getBuilding() != nullptr || tile->getTower() != nullptr) {
        std::cerr << "Tile already has a building or tower.\n";
        return false;
    }
    if (tile->isBlocked()) {
        std::cerr << "Cannot place a building on a blocked tile.\n";
        return false;
    }
    sf::Vector2f isoPos = IsometricUtils::tileToScreen(row, col);
    if (buildingTexture == "../assets/buildings/moontower.png") {
        // Handle tower placement within MapScreen
        // Towers are managed within MapScreen, so no need to instantiate here
        // Just set the building in the map
        auto building = std::make_shared<Building>(nextBuildingId++, isoPos.x, isoPos.y, buildingTexture);
        tile->setBuilding(building);
    } else {
        auto building = std::make_shared<Building>(nextBuildingId++, isoPos.x, isoPos.y, buildingTexture);
        tile->setBuilding(building);
        tile->setBlockStatus(true); // Block the tile after placing the building
        if (buildingTexture == "../assets/buildings/townhall.png") {
            if (row < 0 || row >= rows - 1 || col < 0 || col >= cols - 1) {
                std::cerr << "Invalid tile coordinates for town hall placement.\n";
                return false;
            }
            sf::Vector2f tileCentrePosition = IsometricUtils::tileToScreen(row, col);
            sf::Vector2f buildingPosition = IsometricUtils::tileToScreen(row, col);
            auto building = std::make_shared<Building>(nextBuildingId++, buildingPosition.x, buildingPosition.y, buildingTexture);
            for (int r = row; r <= row + 1; ++r) {
                for (int c = col; c <= col + 1; ++c) {
                    auto tile = getTile(r, c);
                    tile->setBuilding(building);
                    tile->setBlockStatus(true);
                }
            }
        }
        if (buildingTexture == "../assets/walls/brick_wall.png") {
            std::cout << "Wall placed at tile: (" << row << ", " << col << ").\n";
            tile->setType(TileType::Wall);
            tile->setHealth(10); // Set wall health
        }
         // TRAPS

        if (buildingTexture == "../assets/traps/BarrelBomb/barrel.png") {
            std::cout << "Trap placed at tile: (" << row << ", " << col << ").\n";
            addTrap(row, col, buildingTexture);
        }   

        if (buildingTexture == "../assets/buildings/mushroom1.png") {
            std::cout << "Trap placed at tile: (" << row << ", " << col << ").\n";
            // tile->setTrap(buildingTexture);
            addTrap(row, col, buildingTexture);
        }
    }
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
    tile->setType(TileType::Wall);
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
            outFile << static_cast<int>(tile->getType()) << " "
                    << tile->getTexturePath() << " "
                    << tile->getGrassTileIndex() << " "; // Save grass tile index

            auto building = tile->getBuilding();
            if (building) {
                outFile << building->getId() << " "
                        << building->getTexturePath() << "\n";
            } else {
                outFile << "-1 -\n";
            }
        }
    }
    std::cout << "Map saved successfully to " << filename << std::endl;
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
    tiles.resize(rows, std::vector<std::shared_ptr<Tile>>(cols, nullptr));
    
    int tileType, buildingId, grassIndex;
    std::string texturePath;
    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < cols; ++col) {
            inFile >> tileType >> texturePath >> grassIndex >> buildingId;
            auto tile = getTile(row, col);
            tile->setTexturePath(texturePath);
            tile->setType(static_cast<TileType>(tileType));
            tile->setGrassTileIndex(grassIndex); // Load grass tile index
            
            if (buildingId != -1) {
                inFile >> texturePath; // Read the building texture path
                sf::Vector2f isoPos = IsometricUtils::tileToScreen(row, col);
                sf::Vector2f buildingPosition(
                    isoPos.x + Tile::TILE_WIDTH / 2.0f,
                    isoPos.y + Tile::TILE_HEIGHT
                );
                auto building = std::make_shared<Building>(
                    buildingId, buildingPosition.x, buildingPosition.y, texturePath
                );
                tile->setBuilding(building);
            } else {
                tile->setBuilding(nullptr);
            }
        }
    }
    std::cout << "Map loaded successfully from " << filename << std::endl;
    inFile.close();
    saveState();
}


void Map::saveState() {
    GameState currentState(tiles);
    stateManager.saveState(currentState);
}

void Map::undo() {
    if (stateManager.undo()) {
        const GameState* previousState = stateManager.getCurrentState();
        if (previousState) {
            restoreGameState(*previousState);
        }
    } else {
        std::cerr << "No more actions to undo.\n";
    }
}

void Map::redo() {
    if (stateManager.redo()) {
        const GameState* nextState = stateManager.getCurrentState();
        if (nextState) {
            restoreGameState(*nextState);
        }
    } else {
        std::cerr << "No more actions to redo.\n";
    }
}

void Map::restoreGameState(const GameState& state) {
    const auto& tileStates = state.getTileStates();
    for (size_t row = 0; row < rows; ++row) {
        for (size_t col = 0; col < cols; ++col) {
            auto tile = tiles[row][col];
            const auto& tileState = tileStates[row][col];
            if (tile) {
                tile->setTexturePath(tileState.texturePath); // Set texture first
                tile->setType(tileState.type); // Then set type
                if (tileState.buildingId != -1) {
                    // Recreate Building object
                    sf::Vector2f isoPos = IsometricUtils::tileToScreen(row, col);
                    sf::Vector2f buildingPosition = sf::Vector2f(
                        isoPos.x + Tile::TILE_WIDTH / 2.0f,
                        isoPos.y + Tile::TILE_HEIGHT
                    );
                    auto building = std::make_shared<Building>(tileState.buildingId, buildingPosition.x, buildingPosition.y, tileState.buildingTexturePath);
                    tile->setBuilding(building);
                }
                else {
                    tile->setBuilding(nullptr);
                }
            }
            // Debugging output to verify restoration
            std::cout << "Tile (" << tile->getRow() << ", " << tile->getCol() << ") Type: " 
                      << static_cast<int>(tile->getType()) << ", Blocked: " 
                      << tile->isBlocked() << "\n";
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
    if (row < static_cast<int>(tiles.size()) - 1 && !tiles[row + 1][col]->isBlocked())
        neighbors.push_back(tiles[row + 1][col]); // Down
    if (col > 0 && !tiles[row][col - 1]->isBlocked())
        neighbors.push_back(tiles[row][col - 1]); // Left
    if (col < static_cast<int>(tiles[row].size()) - 1 && !tiles[row][col + 1]->isBlocked())
        neighbors.push_back(tiles[row][col + 1]); // Right
    return neighbors;
}

std::shared_ptr<Tile> Map::findNearestWall(int startRow, int startCol) const {
    if (!getTile(startRow, startCol)) {
        std::cerr << "Invalid starting tile coordinates: (" << startRow << ", " << startCol << ").\n";
        return nullptr;
    }

    std::queue<std::tuple<int, int, int>> q; // row, col, distance
    std::set<std::pair<int, int>> visited;
    q.emplace(startRow, startCol, 0);
    visited.emplace(startRow, startCol);

    const std::vector<std::pair<int, int>> directions = {
        {0, 1}, {1, 0}, {0, -1}, {-1, 0}
    };

    while (!q.empty()) {
        auto [row, col, dist] = q.front();
        q.pop();

        auto tile = getTile(row, col);
        if (tile && tile->isBlocked()) {
            return tile;
        }

        for (const auto& [dRow, dCol] : directions) {
            int newRow = row + dRow;
            int newCol = col + dCol;
            if (visited.find({newRow, newCol}) == visited.end() && getTile(newRow, newCol)) {
                q.emplace(newRow, newCol, dist + 1);
                visited.emplace(newRow, newCol);
            }
        }
    }

    return nullptr;
}

// trap code

bool Map::addTrap(int row, int col, const std::string& trapTexture) {
    auto tile = getTile(row, col);
    if (!tile) {
        std::cerr << "Invalid tile coordinates: (" << row << ", " << col << ").\n";
        return false;
    }
    if (tile->getBuilding() != nullptr) {
        std::cerr << "Tile already has a building.\n";
        return false;
    }
    if (tile->isBlocked()) {
        std::cerr << "Cannot place a trap on a blocked tile.\n";
        return false;
    }
    auto trap = std::make_shared<Trap>(trapTexture);
    tile->setTrap(trap);
    saveState();
    // std::cout << "Trap placed at tile: (" << row << ", " << col << ").\n";
    // std::cout << "Trap texture: " << trapTexture << "\n";
    return true;
}