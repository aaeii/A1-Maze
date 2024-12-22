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
  int rows;
  int cols;
  int cells[MAX_SIZE][MAX_SIZE];
} Cave;

typedef struct {
  int x, y;
} Point;

Maze *create_maze();
Cave *create_cave();

void free_maze(Maze *maze);
void free_cave(Cave *cave);
bool load_maze(char *filepath, Maze *maze);
void generate_maze(Maze *maze, int rows, int cols);
void merge_sets(Maze *maze, int i, int cols, int rows, int *sets);
void save_maze(Maze *maze, char *filename);
void load_maze_from_content(char *str, Maze *maze);

bool load_cave(char *filename, Cave *cave);
void update_cave(Cave *cave, int birth_limit, int death_limit,
                 int can_change[MAX_SIZE][MAX_SIZE]);
void initialize_can_change(int can_change[MAX_SIZE][MAX_SIZE], int rows,
                           int cols, int initial_chance);
int count_live_neighbors(Cave *cave, int y, int x);

int get_max_size();

int **create_path();
void free_path(int **path);

bool valid(Maze *maze, int x, int y);
int bfs(Maze *maze, Point start, Point end, int **path);
bool check_presence_of_walls(int i, Maze *, Point, int, int);
void empty_arr(int **a);
void check_neighboring_cells(Maze *, Point, int c[][MAX_SIZE],
                             Point g[MAX_SIZE * MAX_SIZE], Point p[][MAX_SIZE],
                             int *);

#endif