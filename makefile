CC = gcc

CFLAGS = -O3 -std=c11 -Wall -Wextra
DBGFLAGS = -g -O0 -std=c11 -Wall -Wextra

SRC = source/*.c
TEST = test/*.c

BUILD = build
BIN = $(BUILD)/test_matrix
FASE1 = $(BUILD)/fase1
FASE2 = $(BUILD)/fase2

# =========================
# Ejecutar todo
# =========================

all: fase1 fase2
	@echo ""
	@echo "📊 Ejecutando fase1 (resultados en results2.txt)..."
	@./$(FASE1) > results2.txt
	@echo "✅ Fase1 completado"
	@echo "📈 Generando gráfico..."
	@gnuplot scripts/plot2.gnuplot
	@echo "✅ Gráfico generado en plot_fase1.png"
	@echo ""
	@echo "📊 Ejecutando fase2 (resultados en results.txt)..."
	@./$(FASE2) > results.txt
	@echo "✅ Fase2 completado"
	@echo "📈 Generando gráfico..."
	@gnuplot scripts/plot.gnuplot
	@echo "✅ Gráfico generado en plot_fase2.png"

# =========================
# Fase 1 (Clásico vs MC-CUTOFF=32)
# =========================

run1: fase1
	@echo "📊 Ejecutando fase1 (resultados en results2.txt)..."
	@./$(FASE1) > results2.txt
	@echo "✅ Fase1 completado"
	@echo "📈 Generando gráfico..."
	@gnuplot scripts/plot2.gnuplot
	@echo "✅ Gráfico generado en plot_fase1.png"

fase1: fase1.c
	@mkdir -p $(BUILD)
	@echo "🔧 Compiling fase1..."
	$(CC) $(CFLAGS) $(SRC) fase1.c -o $(FASE1) -lm

# =========================
# Fase 2 (Clásico + MC-CUTOFF + Power of 2)
# =========================

run2: fase2
	@echo "📊 Ejecutando fase2 (resultados en results.txt)..."
	@./$(FASE2) > results.txt
	@echo "✅ Fase2 completado"
	@echo "📈 Generando gráfico..."
	@gnuplot scripts/plot.gnuplot
	@echo "✅ Gráfico generado en plot_fase2.png"

fase2: fase2.c
	@mkdir -p $(BUILD)
	@echo "🔧 Compiling fase2..."
	$(CC) $(CFLAGS) $(SRC) fase2.c -o $(FASE2) -lm

# =========================
# Tests con Valgrind
# =========================

tests:
	@mkdir -p $(BUILD)
	@echo "🔧 Compiling test_matrix..."
	$(CC) $(DBGFLAGS) $(SRC) $(TEST) -o $(BIN) -lm
	@echo ""
	@echo "🧪 Running Valgrind..."
	valgrind --leak-check=full --show-leak-kinds=all --error-exitcode=1 $(BIN)
	@echo ""
	@echo "✅ TEST + VALGRIND PASSED"

# =========================
# Limpieza
# =========================

clean:
	rm -rf $(BUILD)