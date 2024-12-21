// GameStateManager.cpp
#include "GameStateManager.hpp"

GameStateManager::GameStateManager(size_t maxHistory_) : maxHistory(maxHistory_) {
    current = states.end();
}

void GameStateManager::saveState(const GameState& state) {
    // If not at the end, erase all states ahead of the current
    if (current != states.end()) {
        states.erase(std::next(current), states.end());
    }

    // Add the new state to the end
    states.emplace_back(state);
    current = std::prev(states.end());

    // Enforce maximum history size
    if (states.size() > maxHistory) {
        states.pop_front();
        current = states.empty() ? states.end() : std::prev(states.end());
    }
}

bool GameStateManager::undo() {
    if (current == states.begin() || states.empty()) {
        // No more states to undo
        return false;
    }
    --current;
    return true;
}

bool GameStateManager::redo() {
    if (current == states.end() || std::next(current) == states.end()) {
        // No more states to redo
        return false;
    }
    ++current;
    return true;
}

const GameState* GameStateManager::getCurrentState() const {
    if (states.empty() || current == states.end()) {
        return nullptr;
    }
    return &(*current);
}