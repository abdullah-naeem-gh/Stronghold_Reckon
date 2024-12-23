// Map.cpp
#include "Map.hpp"
#include "IsometricUtils.hpp"
#include <iostream>
#include <optional>
#include <queue>
#include <tuple>
#include <set>
#include <random>


Map::Map(int rows, int cols, BulletManager& centralBulletManager)
    : rows(rows), cols(cols), nextBuildingId(1), nextTowerId(1), centralBulletManager(centralBulletManager) {
    initializeTiles();
    saveState(); // Save the initial state
}

void Map::initializeTiles() {
    // Initialize the random number generator
    std::random_device rd;  // Obtain a random number from hardware
    std::mt19937 eng(rd()); // Seed the generator

    // Suppose you have 18 different grass textures (0-17)
    std::uniform_int_distribution<> distr(0, 17); 

    // Resize the tiles vector
    tiles.resize(rows, std::vector<std::shared_ptr<Tile>>(cols, nullptr));

    // Iterate over each tile position
    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < cols; ++col) {
            // Initialize grass tiles with a random index
            tiles[row][col] = std::make_shared<Tile>(row, col, TileType::Grass);
            int randomIndex = distr(eng); // Generate a random tile index
            tiles[row][col]->setGrassTileIndex(randomIndex); // Assign the random index
            
            // Set the tile’s isometric position
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

        // auto building = std::make_shared<Building>(nextBuildingId++, isoPos.x, isoPos.y, buildingTexture);
        // auto building = std::make_shared<Building>(nextBuildingId++, row, col, buildingTexture);

        // tile->setBuilding(building);
    } else {
        // auto building = std::make_shared<Building>(nextBuildingId++, isoPos.x, isoPos.y, buildingTexture);
        auto building = std::make_shared<Building>(nextBuildingId++, row, col, buildingTexture);

        tile->setBuilding(building);
        tile->setBlockStatus(true); // Block the tile after placing the building
        if (buildingTexture == "../assets/buildings/townhall.png") {
            if (row < 0 || row >= rows - 1 || col < 0 || col >= cols - 1) {
                std::cerr << "Invalid tile coordinates for town hall placement.\n";
                return false;
            }
            sf::Vector2f tileCentrePosition = IsometricUtils::tileToScreen(row, col);
            sf::Vector2f buildingPosition = IsometricUtils::tileToScreen(row, col);
            // auto building = std::make_shared<Building>(nextBuildingId++, buildingPosition.x, buildingPosition.y, buildingTexture);
            auto building = std::make_shared<Building>(nextBuildingId++, row, col, buildingTexture);
            for (int r = row; r <= row + 1; ++r) {
                for (int c = col; c <= col + 1; ++c) {
                    auto tile = getTile(r, c);
                    tile->setBuilding(building);
                    tile->setBlockStatus(true);
                    tile->setPosition(isoPos.x, isoPos.y);
                }
            }
        }
        if (buildingTexture == "../assets/walls/brick_wall.png") {
            std::cout << "Wall placed at tile: (" << row << ", " << col << ").\n";
            tile->setBuilding(building);
            tile->setType(TileType::Wall);
            tile->setHealth(100); // Set wall health
        }
         // TRAPS
        if (buildingTexture == "../assets/traps/BarrelBomb/barrel.png" || buildingTexture == "../assets/buildings/mushroom1.png") {
            addTrap(row, col, buildingTexture);
            return true;
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

bool Map::addTower(int row, int col, const std::string& selectedBuildingTexture) {
    auto tile = getTile(row, col);
    if (!tile) {
        std::cerr << "Invalid tile coordinates: (" << row << ", " << col << ").\n";
        return false;
    }
    if (tile->getBuilding() != nullptr) {
        std::cerr << "Tile already has a building.\n";
        return false;
    }
    if (tile->getTower() != nullptr) {
        std::cerr << "Tile already has a tower.\n";
        return false;
    }
    if (tile->isBlocked()) {
        std::cerr << "Cannot place a tower on a blocked tile.\n";
        return false;
    }
    sf::Vector2f newTowerPos = IsometricUtils::tileToScreen(row, col);
    std::shared_ptr<Tower> newTower = std::make_shared<Tower>(nextTowerId++, newTowerPos, 200.0f, 1.0f, centralBulletManager, selectedBuildingTexture);
    towers.push_back(newTower);
    tile->setTower(newTower);

    std::cout << "Tower placed at tile: (" << row << ", " << col << "). Saving state...\n";
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

// void Map::restoreGameState(const GameState& state) {
//     const auto& tileStates = state.getTileStates();
//     for (size_t row = 0; row < rows; ++row) {
//         for (size_t col = 0; col < cols; ++col) {
//             auto tile = tiles[row][col];
//             const auto& tileState = tileStates[row][col];
//             if (tile) {
//                 tile->setTexturePath(tileState.texturePath); // Set texture first
//                 tile->setType(tileState.type); // Then set type
//                 tile->setGrassTileIndex(tileState.grassIndex); // Restore grass index
//                 if (tileState.buildingId != -1) {
//                     // Recreate Building object with stored data
//                     sf::Vector2f isoPos = IsometricUtils::tileToScreen(row, col);
//                     sf::Vector2f buildingPosition = sf::Vector2f(
//                         isoPos.x + Tile::TILE_WIDTH / 2.0f,
//                         isoPos.y + Tile::TILE_HEIGHT
//                     );
//                     auto building = std::make_shared<Building>(
//                         tileState.buildingId, buildingPosition.x, buildingPosition.y, tileState.buildingTexturePath
//                     );
//                     tile->setBuilding(building);
//                 } else {
//                     tile->setBuilding(nullptr);
//                 }
//             }
//             // Debugging output to verify restoration
//             std::cout << "Tile (" << tile->getRow() << ", " << tile->getCol() << ") Type: "
//                       << static_cast<int>(tile->getType()) << ", Blocked: "
//                       << tile->isBlocked() << ", GrassIndex: "
//                       << tile->getGrassTileIndex() << "\n";
//         }
//     }
// }

void Map::restoreGameState(const GameState& state) {
    const auto& tileStates = state.getTileStates();
    for (size_t row = 0; row < rows; ++row) {
        for (size_t col = 0; col < cols; ++col) {
            auto tile = tiles[row][col];
            const auto& tileState = tileStates[row][col];
            if (tile) {
                tile->setTexturePath(tileState.texturePath); // Set texture first
                tile->setType(tileState.type); // Then set type
                tile->setGrassTileIndex(tileState.grassIndex); // Restore grass index
                if (tileState.buildingId != -1) {
                    auto building = std::make_shared<Building>(
                        tileState.buildingId, row, col, tileState.buildingTexturePath
                    );
                    tile->setBuilding(building);
                } else {
                    tile->setBuilding(nullptr);
                }
                if (tileState.hasTrap) {
                    auto trap = std::make_shared<Trap>(tileState.trapTexturePath);
                    tile->setTrap(trap);
                } else {
                    tile->setTrap(nullptr);
                }
                if (tileState.hasTower) {
                    // std::cout << "Tower placed at tile: (" << row << ", " << col << ").\n";
                    auto tower = std::make_shared<Tower>(
                        tileState.towerId, sf::Vector2f(), 200.0f, 1.0f, centralBulletManager, tileState.towerTexturePath
                    );
                    tile->setTower(tower);
                } else {
                    tile->setTower(nullptr);
                }
                if (tileState.hasWall) {
                    tile->setType(TileType::Wall);
                    tile->setBlockStatus(true); // Block the tile after placing the building
                    tile->setHealth(100); // Set wall health
                }
            }
            // // Debugging output to verify restoration
            // std::cout << "Tile (" << tile->getRow() << ", " << tile->getCol() << ") Type: "
            //           << static_cast<int>(tile->getType()) << ", Blocked: "
            //           << tile->isBlocked() << ", GrassIndex: "
            //           << tile->getGrassTileIndex() << "\n";
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


std::vector<std::shared_ptr<Tower>> Map::getTowers() const {
    return towers;
}



// void Map::loadTownHallAnimation() {
//     auto texture = TextureManager::getInstance().getTexture("../assets/spritesheets/townhall_animation.png");
//     if (!texture) {
//         std::cerr << "Failed to load townhall animation texture.\n";
//         return;
//     }

//     townHallSprite.setTexture(*texture);

//     // Assuming each frame is 64x64 pixels
//     for (int i = 0; i < 10; ++i) {
//         townHallFrames.push_back(sf::IntRect(i * 64, 0, 64, 64));
//     }

//     sf::Vector2f position = IsometricUtils::tileToScreen(14, 14);
//     townHallSprite.setPosition(position.x, position.y);
//     townHallSprite.setTextureRect(townHallFrames[0]);
// }

// // Update the animation in your Map class
// void Map::update(float deltaTime) {
//     if (playing) {
//         currentTime += deltaTime;
//         if (currentTime >= frameTime) {
//             currentTime = 0.0f;
//             currentFrame++;
//             if (currentFrame >= townHallFrames.size()) {
//                 playing = false;
//                 currentFrame = townHallFrames.size() - 1; // Stop at the last frame
//             }
//             townHallSprite.setTextureRect(townHallFrames[currentFrame]);
//         }
//     }
// }

// // Draw the animation in your Map class
// void Map::draw(sf::RenderWindow& window) const {
//     for (const auto& row : tiles) {
//         for (const auto& tile : row) {
//             tile->draw(window);
//         }
//     }

//     if (playing) {
//         window.draw(townHallSprite);
//     }
// }