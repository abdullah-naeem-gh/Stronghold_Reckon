// Map.cpp
#include "Map.hpp"
#include "IsometricUtils.hpp"
#include <iostream>
#include <optional>
#include <queue>
#include <tuple>
#include <set>

Map::Map(int rows, int cols)
    : rows(rows), cols(cols), nextBuildingId(1) {
    initializeTiles();
    saveState(); // Save the initial state
}

void Map::initializeTiles() {
    tiles.resize(rows, std::vector<std::shared_ptr<Tile>>(cols, nullptr));
    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < cols; ++col) {
            // Initialize each tile as Grass
            tiles[row][col] = std::make_shared<Tile>(row, col, TileType::Grass);
            sf::Vector2f isoPos = IsometricUtils::tileToScreen(row, col);
            tiles[row][col]->setPosition(isoPos.x, isoPos.y);
        }
    }
    // Add edges between neighboring tiles (existing logic)
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

    if (buildingTexture == "../assets/buildings/tower1.png") {
        auto tower = std::make_shared<Tower>(isoPos, 200.0f, 1.0f); // Adjust range and fire rate as needed
        tile->setTower(tower);
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
            sf::Vector2f buildingPosition = tileCentrePosition + sf::Vector2f(Tile::TILE_WIDTH, Tile::TILE_HEIGHT);
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
            tile->setHealth(100); // Set wall health
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
                    << tile->getTexturePath() << " ";
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
    initializeTiles();
    
    int tileType, buildingId;
    std::string texturePath;
    
    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < cols; ++col) {
            inFile >> tileType >> texturePath >> buildingId;
            auto tile = getTile(row, col);
            tile->setTexturePath(texturePath);
            tile->setType(static_cast<TileType>(tileType));
            
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
    if (row < tiles.size() - 1 && !tiles[row + 1][col]->isBlocked())
        neighbors.push_back(tiles[row + 1][col]); // Down
    if (col > 0 && !tiles[row][col - 1]->isBlocked())
        neighbors.push_back(tiles[row][col - 1]); // Left
    if (col < tiles[row].size() - 1 && !tiles[row][col + 1]->isBlocked())
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