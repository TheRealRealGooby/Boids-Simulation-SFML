# Makefile for Boid (birdoid objects) vs Hoid (humanoid objects) architectures

CXX = g++
CXXFLAGS = -Wall -Iinclude -MMD -MP
LDFLAGS = -lsfml-graphics -lsfml-window -lsfml-system

# Final executable names
EXEC_BOID = simulare_boid
EXEC_HOID = simulare_hoid

# Object files for boid simulation
OBJ_BOID = src/main_boid.o src/Boid.o

# Object files for hoid simulation
OBJ_HOID = src/main_hoid.o src/Hoid.o

# Build both executables by default
all: $(EXEC_BOID) $(EXEC_HOID)

# Link boid simulation components
$(EXEC_BOID): $(OBJ_BOID)
	$(CXX) $^ -o $@ $(LDFLAGS)

# Link hoid simulation components
$(EXEC_HOID): $(OBJ_HOID)
	$(CXX) $^ -o $@ $(LDFLAGS)

# Rule for compiling .cpp source files to .o object files
src/%.o: src/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Include dependency files for Boid and Hoid classes
-include src/*.d

# Remove object files, dependency files, and executables
clean:
	rm -f src/*.o src/*.d $(EXEC_BOID) $(EXEC_HOID)
