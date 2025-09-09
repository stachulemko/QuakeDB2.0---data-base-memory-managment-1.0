# QuakeDB 2.0 - Database Memory Management Makefile
# Kompilacja i testowanie projektu

# Kompilator i flagi
CXX = g++
CC = gcc
CXXFLAGS = -std=c++17 -Wall -Wextra -g -O0 -I$(SRC_DIR)
CFLAGS = -std=c11 -Wall -Wextra -g -O0 -I$(SRC_DIR)

# Katalogi
SRC_DIR = src
TEST_DIR = tests
BUILD_DIR = build
BIN_DIR = bin

# Znajdź wszystkie pliki źródłowe rekursywnie
CPP_SOURCES = $(shell find $(SRC_DIR) -name "*.cpp" 2>/dev/null || echo "")
C_SOURCES = $(shell find $(SRC_DIR) -name "*.c" 2>/dev/null || echo "")

# Utwórz obiekty z zachowaniem struktury katalogów
CPP_OBJECTS = $(CPP_SOURCES:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)
C_OBJECTS = $(C_SOURCES:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)
OBJECTS = $(CPP_OBJECTS) $(C_OBJECTS)

# Pliki testów
TEST_SOURCES = $(wildcard $(TEST_DIR)/*.cpp)
TEST_OBJECTS = $(TEST_SOURCES:$(TEST_DIR)/%.cpp=$(BUILD_DIR)/test_%.o)

# Nazwa głównego programu
TARGET = $(BIN_DIR)/quakedb
TEST_TARGET = $(BIN_DIR)/run_tests

# Google Test
GTEST_FLAGS = -lgtest -lgtest_main -pthread

# Domyślny cel
.PHONY: all clean test install-deps dirs help build debug

all: dirs $(TARGET)

# Tworzenie katalogów (również podkatalogi w build)
dirs:
	@mkdir -p $(BUILD_DIR) $(BIN_DIR) $(SRC_DIR) $(TEST_DIR)
	@if [ -n "$(CPP_SOURCES)" ]; then \
		for src in $(CPP_SOURCES); do \
			mkdir -p $(BUILD_DIR)/$$(dirname $${src#$(SRC_DIR)/}); \
		done; \
	fi
	@if [ -n "$(C_SOURCES)" ]; then \
		for src in $(C_SOURCES); do \
			mkdir -p $(BUILD_DIR)/$$(dirname $${src#$(SRC_DIR)/}); \
		done; \
	fi

# Kompilacja głównego programu
$(TARGET): $(OBJECTS)
	@if [ -z "$(OBJECTS)" ]; then \
		echo "Brak plików źródłowych w katalogu $(SRC_DIR)"; \
		echo "Tworzę przykładowy main.cpp..."; \
		echo '#include <iostream>' > $(SRC_DIR)/main.cpp; \
		echo 'int main() { std::cout << "QuakeDB 2.0 działa!" << std::endl; return 0; }' >> $(SRC_DIR)/main.cpp; \
		$(MAKE) all; \
	else \
		echo "Linkowanie głównego programu..."; \
		$(CXX) $(OBJECTS) -o $(TARGET); \
		echo "Program skompilowany: $(TARGET)"; \
	fi

# Kompilacja testów
test: dirs
	@if [ -z "$(TEST_SOURCES)" ]; then \
		echo "Brak plików testowych w katalogu $(TEST_DIR)"; \
		echo "Tworzę przykładowy test..."; \
		mkdir -p $(TEST_DIR); \
		echo '#include <gtest/gtest.h>' > $(TEST_DIR)/sample_test.cpp; \
		echo 'TEST(SampleTest, BasicTest) { EXPECT_EQ(1, 1); }' >> $(TEST_DIR)/sample_test.cpp; \
	fi
	@$(MAKE) $(TEST_TARGET)
	@echo "Uruchamianie testów..."
	./$(TEST_TARGET)

$(TEST_TARGET): $(TEST_OBJECTS) $(filter-out %main.o, $(OBJECTS))
	@echo "Linkowanie testów..."
	$(CXX) $(TEST_OBJECTS) $(filter-out %main.o, $(OBJECTS)) $(GTEST_FLAGS) -o $(TEST_TARGET)
	@echo "Testy skompilowane: $(TEST_TARGET)"

# Kompilacja plików .cpp z src (zachowuje strukturę katalogów)
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	@echo "Kompilowanie $<..."
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Kompilacja plików .c z src (zachowuje strukturę katalogów)
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	@echo "Kompilowanie $<..."
	$(CC) $(CFLAGS) -c $< -o $@

# Kompilacja testów
$(BUILD_DIR)/test_%.o: $(TEST_DIR)/%.cpp
	@mkdir -p $(dir $@)
	@echo "Kompilowanie testu $<..."
	$(CXX) $(CXXFLAGS) -I$(SRC_DIR) -c $< -o $@

# Czyszczenie
clean:
	@echo "Czyszczenie plików..."
	rm -rf $(BUILD_DIR) $(BIN_DIR)
	@echo "Wyczyszczono!"

# Uruchomienie tylko kompilacji bez testów
build: dirs $(TARGET)

# Debug - wyświetla zmienne
debug:
	@echo "=== STRUKTURA PROJEKTU ==="
	@echo "SRC_DIR: $(SRC_DIR)"
	@echo "CPP_SOURCES: $(CPP_SOURCES)"
	@echo "C_SOURCES: $(C_SOURCES)"
	@echo "OBJECTS: $(OBJECTS)"
	@echo "TEST_SOURCES: $(TEST_SOURCES)"
	@echo "TEST_OBJECTS: $(TEST_OBJECTS)"
	@echo "TARGET: $(TARGET)"
	@echo "TEST_TARGET: $(TEST_TARGET)"
	@echo ""
	@echo "=== ZAWARTOŚĆ KATALOGÓW ==="
	@echo "Katalog src/:"
	@ls -la $(SRC_DIR)/ 2>/dev/null || echo "Katalog $(SRC_DIR) nie istnieje"
	@echo ""
	@echo "Katalog tests/:"
	@ls -la $(TEST_DIR)/ 2>/dev/null || echo "Katalog $(TEST_DIR) nie istnieje"

# Inicjalizacja projektu z przykładowymi plikami
init:
	@echo "Inicjalizacja struktury projektu..."
	@mkdir -p $(SRC_DIR) $(TEST_DIR)
	@if [ ! -f $(SRC_DIR)/main.cpp ]; then \
		echo "Tworzę $(SRC_DIR)/main.cpp..."; \
		echo '#include <iostream>' > $(SRC_DIR)/main.cpp; \
		echo '#include "database.h"' >> $(SRC_DIR)/main.cpp; \
		echo '' >> $(SRC_DIR)/main.cpp; \
		echo 'int main() {' >> $(SRC_DIR)/main.cpp; \
		echo '    std::cout << "QuakeDB 2.0 - Database Memory Management System" << std::endl;' >> $(SRC_DIR)/main.cpp; \
		echo '    return 0;' >> $(SRC_DIR)/main.cpp; \
		echo '}' >> $(SRC_DIR)/main.cpp; \
	fi
	@if [ ! -f $(SRC_DIR)/database.h ]; then \
		echo "Tworzę $(SRC_DIR)/database.h..."; \
		echo '#ifndef DATABASE_H' > $(SRC_DIR)/database.h; \
		echo '#define DATABASE_H' >> $(SRC_DIR)/database.h; \
		echo '' >> $(SRC_DIR)/database.h; \
		echo '// QuakeDB 2.0 header file' >> $(SRC_DIR)/database.h; \
		echo '' >> $(SRC_DIR)/database.h; \
		echo '#endif // DATABASE_H' >> $(SRC_DIR)/database.h; \
	fi
	@if [ ! -f $(TEST_DIR)/database_test.cpp ]; then \
		echo "Tworzę $(TEST_DIR)/database_test.cpp..."; \
		echo '#include <gtest/gtest.h>' > $(TEST_DIR)/database_test.cpp; \
		echo '#include "database.h"' >> $(TEST_DIR)/database_test.cpp; \
		echo '' >> $(TEST_DIR)/database_test.cpp; \
		echo 'TEST(DatabaseTest, BasicTest) {' >> $(TEST_DIR)/database_test.cpp; \
		echo '    EXPECT_EQ(1, 1);' >> $(TEST_DIR)/database_test.cpp; \
		echo '}' >> $(TEST_DIR)/database_test.cpp; \
	fi
	@echo "Struktura projektu utworzona!"

# Instalacja zależności (Ubuntu/Debian)
install-deps:
	@echo "Instalowanie zależności..."
	sudo apt-get update
	sudo apt-get install -y libgtest-dev cmake build-essential
	@echo "Zależności zainstalowane!"

# Pomoc
help:
	@echo "QuakeDB 2.0 Makefile - dostępne komendy:"
	@echo ""
	@echo "  make all          - kompiluje główny program"
	@echo "  make build        - to samo co 'all'"
	@echo "  make test         - kompiluje i uruchamia testy"
	@echo "  make clean        - usuwa pliki kompilacji"
	@echo "  make init         - inicjalizuje projekt z przykładowymi plikami"
	@echo "  make install-deps - instaluje Google Test (Ubuntu/Debian)"
	@echo "  make dirs         - tworzy potrzebne katalogi"
	@echo "  make debug        - wyświetla zmienne do debugowania"
	@echo "  make help         - wyświetla tę pomoc"
	@echo ""
	@echo "Struktura katalogów:"
	@echo "  src/    - pliki źródłowe (.cpp, .c, .h, .hpp)"
	@echo "  tests/  - pliki testów Google Test"
	@echo "  build/  - pliki obiektowe (.o)"
	@echo "  bin/    - pliki wykonywalne"