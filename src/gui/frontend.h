#ifndef S21_FRONTEND_H_
#define S21_FRONTEND_H_

#include <ncurses.h>
#include <time.h>
#include <unistd.h>

#include "../maze/backend.h"

#define DIR "examples/"

void render_maze(int start_y, int start_x, int max_height, int max_width, 
    const Maze *maze, int **path, bool is_path);
void main_menu();
void print_menu(int start_y, int start_x);
void f_case(Maze *maze, char *filepath);
void get_filename(int start_y, int start_x, char *filepath);

#endif