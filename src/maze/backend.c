#include "backend.h"

Maze *create_maze() {
  Maze *maze = calloc(1, sizeof(Maze));

  if (maze == NULL) {
      return NULL;
  }

  maze->rows = 0;
  maze->cols = 0;
  memset(maze->vertical_walls, 0, sizeof(maze->vertical_walls));
  memset(maze->horizontal_walls, 0, sizeof(maze->horizontal_walls));

  return maze;
}

void free_maze(Maze *maze) { free(maze); }

bool load_maze(const char *filepath, Maze *maze) {
  bool isLoad = false;
  FILE *file = fopen(filepath, "r");
  if (file != NULL) {
    fscanf(file, "%d %d", &maze->rows, &maze->cols);
    for (int i = 0; i < maze->rows; i++) {
      for (int j = 0; j < maze->cols; j++) {
        fscanf(file, "%d", &maze->vertical_walls[i][j]);
      }
    }
    for (int i = 0; i < maze->rows; i++) {
      for (int j = 0; j < maze->cols; j++) {
        fscanf(file, "%d", &maze->horizontal_walls[i][j]);
      }
    }
    fclose(file);
    isLoad = true;
  }
  return isLoad;
}

void merge_sets(Maze *maze, int i, int cols, int rows, int *sets) {
  int old_set;
  for (int j = 0; j < cols - 1; j++) {  // merging sets randomly
    if (sets[j] != sets[j + 1] && (rand() % 2 == 0 || i == rows - 1)) {
      maze->vertical_walls[i][j] = 0;
      old_set = sets[j + 1];
      for (int k = 0; k < cols; k++) {
        if (sets[k] == old_set) {
          sets[k] = sets[j];
        }
      }
    } else {
      maze->vertical_walls[i][j] = 1;
    }
  }
}

void generate_maze(Maze *maze, int rows, int cols) {
  maze->rows = rows;
  maze->cols = cols;

  int sets[MAX_SIZE];
  int next_set = 0;  // set number
  srand(time(NULL));
  for (int j = 0; j < cols; j++) {  // assigning set number
    sets[j] = next_set++;
  }

  for (int i = 0; i < rows; i++) {  // generating maze each row at once
    merge_sets(maze, i, cols, rows, sets);
    bool set_connected[MAX_SIZE * MAX_SIZE] = {0};
    for (int j = 0; j < cols; j++) {
      if (i == rows - 1) {
        maze->horizontal_walls[i][j] = 1;  // building horizontal border
      } else if (rand() % 2 == 0 ||
                 !set_connected[sets[j]]) {  // each set needs a bottom path
        maze->horizontal_walls[i][j] = 0;
        set_connected[sets[j]] = true;
      } else {
        maze->horizontal_walls[i][j] = 1;
      }
    }
    for (int j = 0; j < cols; j++) {  // numerating new sets...
      if (maze->horizontal_walls[i][j] == 1 && i < rows - 1) {
        sets[j] = next_set++;
      }

      if (j == cols - 1)
        maze->vertical_walls[i][j] = 1;  // ...and building vertical border
    }
  }
}

void save_maze(const Maze *maze, char *filename) {
  FILE *file = fopen(filename, "w");
  if (file != NULL) {
    fprintf(file, "%d %d\n", maze->rows, maze->cols);

    for (int i = 0; i < maze->rows; ++i) {
      for (int j = 0; j < maze->cols; ++j) {
        fprintf(file, "%d ", maze->vertical_walls[i][j]);
      }
      fprintf(file, "\n");
    }
    fprintf(file, "\n");
    for (int i = 0; i < maze->rows; ++i) {
      for (int j = 0; j < maze->cols; ++j) {
        fprintf(file, "%d ", maze->horizontal_walls[i][j]);
      }
      fprintf(file, "\n");
    }
    fclose(file);
  }
}