CC = gcc

CFLAGS = -O3 -std=c11 -Wall -Wextra -Iinterfaces
DBGFLAGS = -g -O0 -std=c11 -Wall -Wextra -Iinterfaces

SRC = source/*.c
TEST = test/*.c

BUILD = build
BIN = $(BUILD)/test_matrix
MAIN = $(BUILD)/main

# =========================
# Compilar y ejecutar main
# =========================

run: main
	@echo "📊 Ejecutando benchmark (resultados en results.txt)..."
	@./$(MAIN) > results.txt
	@echo "✅ Benchmark completado"
	@echo "📈 Generando gráfico..."
	@gnuplot scripts/plot.gnuplot
	@echo "✅ Gráfico generado en plot.png"

main:
	@mkdir -p $(BUILD)
	@echo "🔧 Compiling main..."
	$(CC) $(CFLAGS) $(SRC) main.c -o $(MAIN) -lm

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
