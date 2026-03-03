# GNU C++ Compiler
CXX = g++

# Setăm flag-urile de compilare:
# -Wall - show all warnings
# -Iinclude - search for header files in include folder
CXXFLAGS = -Wall -Iinclude

# Linking flags for SFML library
LDFLAGS = -lsfml-graphics -lsfml-window -lsfml-system

# Source .cpp files
SRC = src/main.cpp src/Boid.cpp

# Final executable file name
TARGET = simulare

# Default rule
all: $(TARGET)

# Executable file construction rule
$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET) $(LDFLAGS)

# 'make clean' rule
clean:
	rm -f $(TARGET)