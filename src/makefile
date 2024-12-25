# Makefile

CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -I../include
LDFLAGS = -L../lib -lsfml-graphics -lsfml-window -lsfml-system
SRC = main.cpp Map.cpp MapScreen.cpp Building.cpp Bullet.cpp BulletManager.cpp GameStateManager.cpp QuadTree.cpp Skeleton.cpp SkeletonSpawn.cpp Tower.cpp Trap.cpp Tile.cpp TextureManager.cpp UIManager.cpp IsometricUtils.cpp GameState.cpp Tank.cpp TankSpawn.cpp Pathfinding.cpp
OBJ = $(SRC:.cpp=.o)
EXEC = prog

.PHONY: all clean

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CXX) $(OBJ) -o $(EXEC) $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(EXEC)