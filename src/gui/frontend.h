#ifndef S21_FRONTEND_H_
#define S21_FRONTEND_H_

#include <ncurses.h>
#include <time.h>
#include <unistd.h>

#include "../maze/backend.h"

#define DIR "examples/"
#define G_CASE_FILE "g_maze.txt"
#define MENU_Y 0
#define MENU_X 80
#define BUFFER 256

void render_maze(int start_y, int start_x, Maze *maze, int **path, bool is_path);
void main_menu();
void print_menu(int start_y, int start_x);
void f_case(Maze *maze, char *filepath);
void g_case(Maze *maze);
void get_filename(int start_y, int start_x, char *filepath);
bool get_dimension(int start_y, int start_x, int *rows, int *cols);

#endif