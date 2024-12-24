#ifndef S21_FRONTEND_H_
#define S21_FRONTEND_H_

#include <ncurses.h>
#include <time.h>
#include <unistd.h>

#include "../maze/backend.h"

#define DIR "examples/"
#define G_CASE_FILE "g_maze.txt"
#define MENU_Y 0
#define MENU_X 0
#define BUFFER 256
#define MAX_HEIGHT 22
#define MAX_WIDTH 115
#define WARN_Y 6
#define WARN_X 40
#define MAZE_Y 4
#define MAZE_X 0
#define CAVE_Y 7
#define CAVE_X 0

int nanosleep(const struct timespec *req, struct timespec *rem);
void msleep(long msec);

void render_maze(int start_y, int start_x, Maze *maze, int **path,
                 bool is_path);
void main_menu();
void print_menu(int start_y, int start_x);
void f_case(Maze *maze, char *filepath);
void g_case(Maze *maze);
void c_case(Cave *cave, char *filepath);
void p_case(Point start, Point end, Maze *maze, int **path);
bool get_cave_info(int start_y, int start_x, int *birth_limit, int *death_limit,
                   int *initial_chance, int *mode, int *delay);
void step_by_step_mode(Cave *cave, int birth_limit, int death_limit,
                       int can_change[MAX_SIZE][MAX_SIZE]);
void automatic_mode(Cave *cave, int birth_limit, int death_limit, int delay,
                    int can_change[MAX_SIZE][MAX_SIZE]);
void get_filename(int start_y, int start_x, char *filepath);
bool get_dimension(int start_y, int start_x, int *rows, int *cols);
void render_cave(int start_y, int start_x, Cave *cave);

#endif