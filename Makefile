# Makefile para Práctica 1: Probabilidad Condicionada
# Universidad de La Laguna - Inteligencia Artificial Avanzada

CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2
TARGET = inference_demo
SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin

# Archivos fuente
SOURCES = $(SRC_DIR)/main.cc \
          $(SRC_DIR)/distribution/binary_distribution/binary_distribution.cc \
          $(SRC_DIR)/conditional_inference_engine/conditional_inference_engine.cc

# Archivos objeto
OBJECTS = $(OBJ_DIR)/main.o \
          $(OBJ_DIR)/binary_distribution.o \
          $(OBJ_DIR)/conditional_inference_engine.o

# Regla principal
all: directories $(BIN_DIR)/$(TARGET)

# Crear directorios necesarios
directories:
	@mkdir -p $(OBJ_DIR) $(BIN_DIR)

# Enlazar ejecutable
$(BIN_DIR)/$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^
	@echo "✓ Compilación completada: $(BIN_DIR)/$(TARGET)"

# Compilar main.cc
$(OBJ_DIR)/main.o: $(SRC_DIR)/main.cc
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Compilar binary_distribution.cc
$(OBJ_DIR)/binary_distribution.o: $(SRC_DIR)/distribution/binary_distribution/binary_distribution.cc
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Compilar conditional_inference_engine.cc
$(OBJ_DIR)/conditional_inference_engine.o: $(SRC_DIR)/conditional_inference_engine/conditional_inference_engine.cc
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Limpiar archivos generados
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR) test_distribution.csv
	@echo "✓ Archivos de compilación eliminados"

# Ejecutar el programa
run: all
	@echo "\n▶ Ejecutando el programa...\n"
	@./$(BIN_DIR)/$(TARGET)

# Compilar con información de depuración
debug: CXXFLAGS += -g -DDEBUG
debug: clean all

.PHONY: all clean run debug directories
