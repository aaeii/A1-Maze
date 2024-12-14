#include "maze.h"

int main() {
  srand(time(NULL));
  initscr();
  noecho();
  cbreak();
  keypad(stdscr, TRUE);
  curs_set(0);
  int key = 0;
  while (key != 27) {
    Maze *maze = calloc(1, sizeof(Maze));
    inizialization_maze(maze);
    // clear();
    char filename[30];
    print_menu();
    refresh();
    key = getch();
    if (key == 'l' || key == 'L') {
      mvprintw(top_indentation + 5, indentation_from_field, "Enter filename: ");
      memset(filename, 0, sizeof(filename));
      echo();
      getnstr(filename, 30 - 1);
      noecho();
      if (!load_maze(filename, maze)) {
        mvprintw(top_indentation + 6, indentation_from_field,
                 "The file could not be uploaded\n");
      } else {
        clear();
        draw_maze(maze);
      }
      refresh();
    }
    if (key == 'g' || key == 'G') {
      mvprintw(top_indentation + 5, indentation_from_field,
               "Enter rows and cols: ");
      refresh();
    }
    free(maze);
  }

  endwin();
  return 0;
}

int load_maze(char *filename, Maze *maze) {
  int flag = 0;
  FILE *file = fopen(filename, "r");
  if (file) {
    fscanf(file, "%d %d", &maze->rows, &maze->cols);
    for (int i = 0; i < maze->rows; i++) {
      for (int j = 0; j < maze->cols; j++) {
        fscanf(file, "%d", &maze->vertical[i][j]);
      }
    }
    for (int i = 0; i < maze->rows; i++) {
      for (int j = 0; j < maze->cols; j++) {
        fscanf(file, "%d", &maze->horizontal[i][j]);
      }
    }
    fclose(file);
    flag = 1;
  }
  return flag;
}

void inizialization_maze(Maze *maze) {
  maze->rows = 0;
  maze->cols = 0;
  memset(maze->vertical, 0, sizeof(maze->vertical));
  memset(maze->horizontal, 0, sizeof(maze->horizontal));
}

void save_maze(char *filename, int r, int c, int **v, int **h) {
  FILE *file = fopen(filename, "w");

  fprintf(file, "%d %d\n", r, c);
  for (int i = 0; i < r; i++) {
    for (int j = 0; j < c; j++) {
      fprintf(file, "%d ", v[i][j]);
    }
    fprintf(file, "\n");
  }
  fprintf(file, "\n");
  for (int i = 0; i < r; i++) {
    for (int j = 0; j < c; j++) {
      fprintf(file, "%d ", h[i][j]);
    }
    fprintf(file, "\n");
  }
  fclose(file);
}

// interface
void print_menu() {
  mvprintw(top_indentation, indentation_from_field, "L - load from file");
  mvprintw(top_indentation + 1, indentation_from_field, "G - generate");
  mvprintw(top_indentation + 2, indentation_from_field, "Esc - exit");
}

void draw_maze(Maze *maze) {
  for (int i = 0; i < maze->rows; i++) {
    for (int j = 0; j < maze->cols; j++) {
      if (maze->horizontal[i][j] == 1) {
        mvaddch(i + 1, j * 2, '_');
      }
      if (maze->vertical[i][j] == 1) {
        mvaddch(i + 1, j * 2 + 1, '|');
      }
      if (j == maze->cols - 1) {
        mvaddch(i + 1, j * 2 + 2, ' ');
      }
    }
  }
}