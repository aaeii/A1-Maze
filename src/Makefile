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

# Папки с исходными файлами
BACKEND_DIR = maze
FRONTEND_DIR = gui

# Папка для объектных файлов
OUT_DIR = out

# Исходные файлы
BACKEND_SRC = $(BACKEND_DIR)/backend.c
FRONTEND_SRC = $(FRONTEND_DIR)/frontend.c

# Объектные файлы
OBJS = $(OUT_DIR)/backend.o $(OUT_DIR)/frontend.o

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

run: $(BUILD_DIR)/$(TARGET)
	$(BUILD_DIR)/$(TARGET)

format:
	cp ../materials/linters/.clang-format ../src/.clang-format
	clang-format -style=Google -n gui/*.c gui/*.h
	clang-format -style=Google -n maze/*.c maze/*.h
#	clang-format -style=Google -n test/*.c test/*.h

	clang-format -style=Google -i gui/*.c gui/*.h
	clang-format -style=Google -i maze/*.c maze/*.h
#	clang-format -style=Google -i test/*.c test/*.h
	rm .clang-format	

# Очистка
clean:
	rm -rf $(OUT_DIR) $(BUILD_DIR)
	rm g_maze.txt