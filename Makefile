# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2
CPPFLAGS = -Isrc

# Directories
SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin

# Source files
SOURCES = $(SRC_DIR)/main.cc \
			$(SRC_DIR)/conditional_inference_engine/conditional_inference_engine.cc \
			$(SRC_DIR)/distribution/binary_distribution/binary_distribution.cc \
			$(SRC_DIR)/performance_analyzer/performance_analyzer.cc

# Object files
OBJECTS = $(patsubst $(SRC_DIR)/%.cc, $(OBJ_DIR)/%.o, $(SOURCES))

# Target executables
TARGETS = $(BIN_DIR)/inference_demo $(BIN_DIR)/inference_interactive

# Default target
all: $(TARGETS)

# Create directories
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)/conditional_inference_engine $(OBJ_DIR)/distribution/binary_distribution $(OBJ_DIR)/performance_analyzer $(BIN_DIR)

# Compile object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cc | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@

# Link executables (adjust based on your main.cc)
$(BIN_DIR)/inference_demo: $(OBJECTS)
	$(CXX) $(CXXFLAGS) $^ -o $@

$(BIN_DIR)/inference_interactive: $(OBJECTS)
	$(CXX) $(CXXFLAGS) $^ -o $@

# Clean build artifacts
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

# Phony targets
.PHONY: all clean