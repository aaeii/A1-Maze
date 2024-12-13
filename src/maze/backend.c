#include "backend.h"

Maze *create_maze() {
  Maze *maze = calloc(1, sizeof(Maze));
  maze->rows = 0;
  maze->cols = 0;
  memset(maze->vertical_walls, 0, sizeof(maze->vertical_walls));
  memset(maze->horizontal_walls, 0, sizeof(maze->horizontal_walls));
  return maze;
}

void free_maze(Maze *maze) { free(maze); }

bool load_maze(const char *filename, Maze *maze) {
  bool isLoad = 0;
  FILE *file = fopen(filename, "r");
  if (file != NULL) {
    fscanf(file, "%d %d", &maze->rows, &maze->cols);
    for (int i = 0; i < maze->rows; ++i) {
      for (int j = 0; j < maze->cols; ++j) {
        fscanf(file, "%d", &maze->vertical_walls[i][j]);
      }
    }
    for (int i = 0; i < maze->rows; ++i) {
      for (int j = 0; j < maze->cols; ++j) {
        fscanf(file, "%d", &maze->horizontal_walls[i][j]);
      }
    }
    fclose(file);
    isLoad = 1;
  }
  return isLoad;
}