# Variables de compilación
CXX = g++
TARGETS = strassen naive

# Direcciones
SRC_DIR = ./algorithms
MEASUREMENTS = ./data/measurements

# Regla principal
all: $(TARGETS)

# Compilación individual
strassen: $(SRC_DIR)/strassen.cpp
	$(CXX) $(SRC_DIR)/strassen.cpp -o $(SRC_DIR)/strassen

naive: $(SRC_DIR)/naive.cpp
	$(CXX) $(SRC_DIR)/naive.cpp -o $(SRC_DIR)/naive

run: all
	@echo "Iniciando batería de pruebas completa..."
	@( \
		echo ">>> Ejecutando Fase 1: NAIVE"; \
		$(SRC_DIR)/naive > $(MEASUREMENTS)/reporte_naive.txt; \
		\
		echo ">>> Ejecutando Fase 2: STRASSEN"; \
		$(SRC_DIR)/strassen > $(MEASUREMENTS)/reporte_strassen.txt; \
		\
	) & \
	PID=$$!; \
	( sleep 300; if ps -p $$PID > /dev/null; then echo "\n[AVISO]: El proceso global lleva más de 5 minutos..."; fi ) & \
	wait $$PID
	@echo "-------------------------------------------"
	@echo "TODAS LAS PRUEBAS FINALIZADAS EXITOSAMENTE"
	@echo "Reportes generados en: $(MEASUREMENTS)/"
	@echo "-------------------------------------------"

clean:
	rm -f $(SRC_DIR)/strassen $(SRC_DIR)/naive $(MEASUREMENTS)/reporte_*.txt