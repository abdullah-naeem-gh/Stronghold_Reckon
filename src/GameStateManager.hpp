// GameStateManager.hpp
#ifndef GAMESTATEMANAGER_HPP
#define GAMESTATEMANAGER_HPP

#include "GameState.hpp"
#include <list>
#include <memory>

class GameStateManager {
public:
    GameStateManager(size_t maxHistory_ = 100); // Optional: Limit history size

    // Saves a new state and discards any states ahead of the current position
    void saveState(const GameState& state);

    // Undoes the last action, returns true if successful
    bool undo();

    // Redoes the last undone action, returns true if successful
    bool redo();

    // Retrieves the current state
    const GameState* getCurrentState() const;

private:
    std::list<GameState> states;
    std::list<GameState>::iterator current;
    size_t maxHistory;
};

#endif // GAMESTATEMANAGER_HPP