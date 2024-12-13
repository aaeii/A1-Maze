#ifndef S21_FRONTEND_H_
#define S21_FRONTEND_H_

#include <ncurses.h>
#include <time.h>
#include <unistd.h>

#include "../maze/backend.h"

void render_maze(int start_y, int start_x, int max_height, int max_width, 
    const Maze *maze, int **path, bool is_path);
void main_menu();

#endif