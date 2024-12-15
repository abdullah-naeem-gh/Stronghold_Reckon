# Makefile

CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -I../include
LDFLAGS = -L../lib -lsfml-graphics -lsfml-window -lsfml-system
SRC = main.cpp Map.cpp MapScreen.cpp Building.cpp Tile.cpp TextureManager.cpp UIManager.cpp IsometricUtils.cpp GameState.cpp
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