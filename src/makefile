# Compiler
CXX = g++

# Compiler Flags
# -std=c++11 : Use the C++11 standard
# -Wall       : Enable all common warnings
# -Wextra     : Enable extra warnings
CXXFLAGS = -std=c++11 -Wall -Wextra -I../include

# Linker Flags
LDFLAGS = -L../lib -lsfml-graphics -lsfml-window -lsfml-system

# Source Files
SRC = main.cpp Map.cpp MapScreen.cpp Building.cpp

# Object Files
OBJ = $(SRC:.cpp=.o)

# Executable Name
EXEC = Game

# Phony Targets
.PHONY: all clean

# Default Target
all: $(EXEC)

# Link Object Files to Create Executable
$(EXEC): $(OBJ)
	$(CXX) $(OBJ) -o $(EXEC) $(LDFLAGS)

# Compile Source Files to Object Files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean Build Files
clean:
	rm -f $(OBJ) $(EXEC)