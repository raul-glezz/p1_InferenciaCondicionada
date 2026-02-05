# Makefile para Práctica 1: Probabilidad Condicionada
# Universidad de La Laguna - Inteligencia Artificial Avanzada

CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2
TARGET_DEMO = inference_demo
TARGET_INTERACTIVE = inference_interactive
SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin

# Archivos fuente
SOURCES_DEMO = $(SRC_DIR)/main.cc \
          $(SRC_DIR)/distribution/binary_distribution/binary_distribution.cc \
          $(SRC_DIR)/conditional_inference_engine/conditional_inference_engine.cc

SOURCES_INTERACTIVE = $(SRC_DIR)/main_interactivo.cc \
          $(SRC_DIR)/distribution/binary_distribution/binary_distribution.cc \
          $(SRC_DIR)/conditional_inference_engine/conditional_inference_engine.cc

# Archivos objeto
OBJECTS_DEMO = $(OBJ_DIR)/main.o \
          $(OBJ_DIR)/binary_distribution.o \
          $(OBJ_DIR)/conditional_inference_engine.o

OBJECTS_INTERACTIVE = $(OBJ_DIR)/main_interactivo.o \
          $(OBJ_DIR)/binary_distribution.o \
          $(OBJ_DIR)/conditional_inference_engine.o

# Regla principal
all: directories $(BIN_DIR)/$(TARGET_DEMO) $(BIN_DIR)/$(TARGET_INTERACTIVE)

# Crear directorios necesarios
directories:
	@mkdir -p $(OBJ_DIR) $(BIN_DIR)

# Enlazar ejecutable de demostración
$(BIN_DIR)/$(TARGET_DEMO): $(OBJECTS_DEMO)
	$(CXX) $(CXXFLAGS) -o $@ $^
	@echo "✓ Compilación completada: $(BIN_DIR)/$(TARGET_DEMO)"

# Enlazar ejecutable interactivo
$(BIN_DIR)/$(TARGET_INTERACTIVE): $(OBJECTS_INTERACTIVE)
	$(CXX) $(CXXFLAGS) -o $@ $^
	@echo "✓ Compilación completada: $(BIN_DIR)/$(TARGET_INTERACTIVE)"

# Compilar main.cc
$(OBJ_DIR)/main.o: $(SRC_DIR)/main.cc
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Compilar main_interactivo.cc
$(OBJ_DIR)/main_interactivo.o: $(SRC_DIR)/main_interactivo.cc
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

# Ejecutar el programa de demostración
run: all
	@echo "\n▶ Ejecutando demostración automática...\n"
	@./$(BIN_DIR)/$(TARGET_DEMO)

# Ejecutar el programa interactivo
interactive: all
	@echo "\n▶ Ejecutando modo interactivo...\n"
	@./$(BIN_DIR)/$(TARGET_INTERACTIVE)

# Compilar con información de depuración
debug: CXXFLAGS += -g -DDEBUG
debug: clean all

.PHONY: all clean run interactive debug directories
