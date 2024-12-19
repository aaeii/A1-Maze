TARGET = maze
BUILD_DIR = build
CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c11 -Wpedantic
LDFLAGS = -lncurses
BACKEND_DIR = maze
FRONTEND_DIR = gui
OUT_DIR = out
BACKEND_SRC = $(BACKEND_DIR)/backend.c
FRONTEND_SRC = $(FRONTEND_DIR)/frontend.c
OBJS = $(OUT_DIR)/backend.o $(OUT_DIR)/frontend.o

all: install run

install: uninstall $(OUT_DIR) $(BUILD_DIR) $(BUILD_DIR)/$(TARGET)

uninstall: clean
	rm -rf $(BUILD_DIR)

$(BUILD_DIR)/$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(OUT_DIR)/backend.o: $(BACKEND_SRC)
	$(CC) $(CFLAGS) -c $< -o $@

$(OUT_DIR)/frontend.o: $(FRONTEND_SRC)
	$(CC) $(CFLAGS) -c $< -o $@

$(OUT_DIR):
	mkdir -p $(OUT_DIR)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

run: $(BUILD_DIR)/$(TARGET)
	$(BUILD_DIR)/$(TARGET)

style:
	clang-format -style=Google -n maze/*.c maze/*.h test/*.c test/*.h

format:
	clang-format -style=Google -i gui/*.c gui/*.h maze/*.c maze/*.h
#	clang-format -style=Google -i test/*.c test/*.h

clean:
	rm -rf $(OUT_DIR) g_maze.txt