# Stronghold Reckoning

"Stronghold Reckoning" is a strategic 2D tower defense game where players must defend their treasure vault from waves of enemies. Build defenses, manage resources, and make strategic decisions in real-time to survive and conquer.

---

## Game Overview

- **Objective**: Defend your treasury from attacking enemies while expanding and upgrading your tower.
- **Resources**: Collect Gold and Mana to build and upgrade towers and cast spells.
- **Game Engine**: Developed using SFML with C++11 for rendering, events, and input management.

---

## Gameplay Mechanics

### Setup and Phases

1. **Starting the Game**
   - Begin with a basic tower and limited resources.
   - Strategically place defenses on a 2D grid map.

2. **Defense Mechanisms**
   - Build traps and towers with special abilities.
   - Use power-ups and spells to enhance your defenses.

3. **Enemy AI and Pathfinding**
   - Enemies utilize advanced A* and BFS algorithms for pathfinding.
   - Adapt to various enemy types, including:
     - Basic grunts
     - Tank enemies
     - Stealth units
     - Flying units

4. **Player Attack Mode**
   - Counterattack opportunities arise after surviving enemy waves.
   - Prioritize weak points in enemy defenses.

5. **Unique Features**
   - Dynamic environment with evolving map conditions.
   - Procedural generation of enemy waves for replayability.
   - State management using complex data structures for strategic depth.

---

## Development Setup

### Prerequisites

- **SFML**: Ensure SFML is installed on your system.
- **C++ Compiler**: Compatible with C++11. Recommended: `g++` or `clang++`.

### Building the Project

Navigate to the `src/` directory and compile the project using the provided `makefile`:

```bash
cd src/
make
```

### Running the Game

After successful compilation, run the executable:

```bash
./prog
```

---

## Game Controls

- **W/S**: Move camera up/down.
- **A/D**: Move camera left/right.
- **L**: Load a saved map.
- **O**: Save the current map.
- **Z/Y**: Undo/Redo actions using the stack.

---

## Concept and Mechanics

The game incorporates advanced data structures and algorithms:

- **Linked Lists**: Used for undo/redo actions.
- **Priority Queues** and **Graphs**: Power the enemy pathfinding.
- **Heaps**: Manage dynamic game state.
- **Stacks and Queues**: Organize defenses and enemy waves.

---

## Contributions

We welcome contributions! To contribute:

1. Fork the repository on GitHub.
2. Make your changes.
3. Open a pull request.

For updates and improvements, please check the GitHub repository. Refer to the in-code comments for additional information and asset management.

---

## Notes

- SFML prefers PNG files over JPG for better compatibility.
- Tile and building dimensions are standardized:
  - **Tiles**: 64x32 pixels
  - **Buildings**: 64x64 pixels

---

Enjoy building and defending in *Stronghold Reckoning*! If you encounter any issues, feel free to report them on GitHub.
