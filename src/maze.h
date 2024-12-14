#include "ncurses.h"
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_SIZE 50
#define indentation_from_field 70
#define top_indentation 0
#define ESCAPE 27

typedef struct {
  int rows;
  int cols;
  int vertical[MAX_SIZE][MAX_SIZE];
  int horizontal[MAX_SIZE][MAX_SIZE];
} Maze;

void inizialization_maze(Maze *maze);
int load_maze(char *filename, Maze *maze);
void save_maze(char *filename, int r, int c, int **v, int **h);
void print_menu();
void draw_maze(Maze *maze);
