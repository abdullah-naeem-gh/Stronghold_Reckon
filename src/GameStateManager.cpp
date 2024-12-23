#include "GameStateManager.hpp"

// Constructor initializes stateManager with maxHistory and sets current to end
GameStateManager::GameStateManager(size_t maxHistory_) : maxHistory(maxHistory_) {
    current = states.end();
}

// Saves a new state and discards any states ahead of the current position
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

// Undoes the last action, returns true if successful
bool GameStateManager::undo() {
    if (current == states.begin() || states.empty()) {
        // No more states to undo
        return false;
    }
    --current;
    return true;
}

// Redoes the last undone action, returns true if successful
bool GameStateManager::redo() {
    if (current == states.end() || std::next(current) == states.end()) {
        // No more states to redo
        return false;
    }
    ++current;
    return true;
}

// Retrieves the current state
const GameState* GameStateManager::getCurrentState() const {
    if (states.empty() || current == states.end()) {
        return nullptr;
    }
    return &(*current);
}