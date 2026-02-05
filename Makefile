CXX := g++
CXXFLAGS := -std=c++23
LDFLAGS := -lstdc++

SRCDIR := src
OBJDIR := obj

# Descubre todos los .cc recursivamente bajo src
SRC := $(shell find $(SRCDIR) -name '*.cc')
# Genera los objetos correspondientes bajo obj, reflejando la estructura de directorios
OBJ := $(patsubst $(SRCDIR)/%.cc, $(OBJDIR)/%.o, $(SRC))

BIN := p1_InferenciaCondicionada

.PHONY: all clean

all: $(BIN)

$(BIN): $(OBJ)
	@echo "Enlazando $^ --> $@"
	@$(CXX) $^ -o $@ $(LDFLAGS)

# Regla genérica: compila cualquier src/%.cc a obj/%.o, creando subdirectorios si hace falta
$(OBJDIR)/%.o: $(SRCDIR)/%.cc
	@echo "Compilando $< --> $@"
	@mkdir -p $(dir $@)
	@$(CXX) $(CXXFLAGS) -I$(SRCDIR) -c $< -o $@

run: $(BIN)
	@echo "Configuraciones disponibles en data/:"; \
	ls -1 data/config*.txt 2>/dev/null || echo "No hay configuraciones disponibles."; \
	echo ""; \
	while true; do \
		read -p "Ingrese el número de configuración: " number; \
		input="data/config$${number}.txt"; \
		if [ -f "$$input" ]; then \
			break; \
		else \
			echo "Error: No existe el archivo $$input."; \
			echo ""; \
			echo "Configuraciones disponibles en data/:"; \
			ls -1 data/config*.txt 2>/dev/null || echo "No hay configuraciones disponibles."; \
			echo ""; \
		fi; \
	done; \
	./$(BIN) "$$input"; \
	echo ""; \

clean:
	@echo "Limpiando..."
	@rm -rf $(OBJDIR) $(BIN)