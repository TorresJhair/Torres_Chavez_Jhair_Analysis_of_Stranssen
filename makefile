CC = gcc

CFLAGS = -O2 -std=c11 -Wall -Wextra -Iinclude
DBGFLAGS = -g -O0 -std=c11 -Wall -Wextra -Iinclude

SRC = source/matrix.c
TEST = test/test_matrix.c

BUILD = build
BIN = $(BUILD)/test_matrix

# =========================
# ÚNICO comando
# =========================

tests:
	@mkdir -p $(BUILD)

	@echo "🔧 Compiling test_matrix..."
	$(CC) $(DBGFLAGS) $(SRC) $(TEST) -o $(BIN)

	@echo ""
	@echo "🚀 Running test..."
	$(BIN)

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