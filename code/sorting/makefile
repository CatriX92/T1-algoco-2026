# Variables de compilación
CXX = g++
CXXFLAGS = -std=c++17 -O3
TARGETS = sort mergesort quicksort 

# Direcciones
SRC_DIR = ./algorithms
MEASUREMENTS = ./data/measurements

# Regla principal
all: $(TARGETS)

# Compilación individual
quicksort: $(SRC_DIR)/quicksort.cpp
	$(CXX) $(CXXFLAGS) $(SRC_DIR)/quicksort.cpp -o $(SRC_DIR)/quicksort

mergesort: $(SRC_DIR)/mergesort.cpp
	$(CXX) $(CXXFLAGS) $(SRC_DIR)/mergesort.cpp -o $(SRC_DIR)/mergesort

sort: $(SRC_DIR)/sort.cpp
	$(CXX) $(CXXFLAGS) $(SRC_DIR)/sort.cpp -o $(SRC_DIR)/sort

# Ejecución secuencial completa
run: all
	@echo "Iniciando batería de pruebas completa..."
	@( \
		echo ">>> Ejecutando Fase 1: SORT"; \
		$(SRC_DIR)/sort > $(MEASUREMENTS)/reporte_sort.txt; \
		\
		echo ">>> Ejecutando Fase 2: MERGESORT"; \
		$(SRC_DIR)/mergesort > $(MEASUREMENTS)/reporte_mergesort.txt; \
		\
		echo ">>> Ejecutando Fase 3: QUICKSORT"; \
		$(SRC_DIR)/quicksort > $(MEASUREMENTS)/reporte_quicksort.txt; \
	) & \
	PID=$$!; \
	( sleep 300; if ps -p $$PID > /dev/null; then echo "\n[AVISO]: El proceso global lleva más de 5 minutos..."; fi ) & \
	wait $$PID
	@echo "-------------------------------------------"
	@echo "TODAS LAS PRUEBAS FINALIZADAS EXITOSAMENTE"
	@echo "Reportes generados en: $(MEASUREMENTS)/"
	@echo "-------------------------------------------"

clean:
	rm -f $(SRC_DIR)/quicksort $(SRC_DIR)/mergesort $(SRC_DIR)/sort $(MEASUREMENTS)/reporte_*.txt