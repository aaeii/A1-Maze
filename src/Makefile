# Название исполняемого файла
TARGET = maze

# Папка для исполняемого файла
BUILD_DIR = build

# Компилятор
CC = gcc

# Флаги компиляции
CFLAGS = -Wall -Wextra -Werror -std=c11 -Wpedantic

# Флаги линковки
LDFLAGS = -lncurses

# Флаги библиотеки check
CHECK_LIBS = -lcheck -lpthread -lrt -lsubunit -lm

# Папки с исходными файлами
BACKEND_DIR = maze
FRONTEND_DIR = gui
TEST_DIR = test

# Папка для объектных файлов
OUT_DIR = out

# Исходные файлы
BACKEND_SRC = $(BACKEND_DIR)/backend.c
FRONTEND_SRC = $(FRONTEND_DIR)/frontend.c
TEST_SRCS = $(TEST_DIR)/tests.c

# Объектные файлы
OBJS = $(OUT_DIR)/backend.o $(OUT_DIR)/frontend.o

# Исполняемый файл тестов
TEST_TARGET = test_maze

# Флаги valgrind
TST_CFLAGS:= -g $(CFLAGS)
VALGRIND_FLAGS=--trace-children=yes --track-fds=yes --track-origins=yes --leak-check=full --show-leak-kinds=all --verbose

all: install run

# Правило по умолчанию
install: $(OUT_DIR) $(BUILD_DIR) $(BUILD_DIR)/$(TARGET)

# Правило для компиляции исполняемого файла
$(BUILD_DIR)/$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# Правило для компиляции .c файлов в .o файлы
$(OUT_DIR)/backend.o: $(BACKEND_SRC)
	$(CC) $(CFLAGS) -c $< -o $@

$(OUT_DIR)/frontend.o: $(FRONTEND_SRC)
	$(CC) $(CFLAGS) -c $< -o $@

# Создание папки out, если она не существует
$(OUT_DIR):
	mkdir -p $(OUT_DIR)

# Создание папки build, если она не существует
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

tests: $(TEST_SRCS)
	@$(CC) $(CFLAGS) $(TEST_SRCS) $(BACKEND_SRC) -o $(TEST_TARGET) $(CHECK_LIBS)
	@./$(TEST_TARGET)

gcov_report: test
	$(CC) $(CFLAGS) -fprofile-arcs -ftest-coverage $(BACKEND_SRC) $(TEST_SRCS) -o tests $(CHECK_LIBS)
	./tests
	lcov -t "t_maze" -o s21_test.info -c -d .
	genhtml -o report s21_test.info
	xdg-open report/index.html
	rm -f *.o
	rm -f *.gcda
	rm -f *.gcno
	rm -f *.txt
	rm -f tests	

run: $(BUILD_DIR)/$(TARGET)
	$(BUILD_DIR)/$(TARGET)

format:
	cp ../materials/linters/.clang-format ../src/.clang-format
	clang-format -style=Google -n gui/*.c gui/*.h
	clang-format -style=Google -n maze/*.c maze/*.h
	clang-format -style=Google -n test/*.c

	clang-format -style=Google -i gui/*.c gui/*.h
	clang-format -style=Google -i maze/*.c maze/*.h
	clang-format -style=Google -i test/*.c
	rm .clang-format


valgrind:
	$(CC) $(TST_CFLAGS) $(BACKEND_SRC) $(TEST_SRCS) $< -o val_test.out $(CHECK_LIBS)
	CK_FORK=no valgrind $(VALGRIND_FLAGS) --log-file=RESULT_VALGRIND.txt ./val_test.out	

clean:
	rm -rf $(OUT_DIR) $(BUILD_DIR)
	rm -rf report
	rm test_maze val_test.out RESULT_VALGRIND.txt s21_test.info g_maze.txt test_maze
	