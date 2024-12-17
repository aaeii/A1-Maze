#ifndef S21_BACKEND_H_
#define S21_BACKEND_H_

#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_SIZE 50

typedef struct {
  int rows;
  int cols;
  int vertical_walls[MAX_SIZE][MAX_SIZE];
  int horizontal_walls[MAX_SIZE][MAX_SIZE];
} Maze;

typedef struct {
  int x, y;
} Point;

Maze *create_maze();
void free_maze(Maze *maze);
bool load_maze(const char *filepath, Maze *maze);
void generate_maze(Maze *maze, int rows, int cols);
void merge_sets(Maze *maze, int i, int cols, int rows, int *sets);
void save_maze(const Maze *maze, char *filename);

#endif