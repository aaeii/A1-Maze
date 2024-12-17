#include "frontend.h"

#include "../maze/backend.h"

void render_maze(int start_y, int start_x, Maze *maze, int **path,
                 bool is_path) {
  int cell_height = MAX_HEIGHT / maze->rows;
  int cell_width = MAX_WIDTH / maze->cols;
  int x = 0;
  int y = 0;

  mvhline(start_y, start_x, '_', MAX_WIDTH);
  mvvline(start_y + 1, start_x, '|', MAX_HEIGHT);

  for (int i = 0; i < maze->rows; i++) {
    for (int j = 0; j < maze->cols; j++) {
      y = start_y + i * cell_height;
      x = start_x + j * cell_width;
      if (is_path && path[i][j]) {
        attron(COLOR_PAIR(1));
        if (i > 0 && path[i - 1][j] && !maze->horizontal_walls[i - 1][j])
          mvvline(y - cell_height / 2, x + cell_width / 2, '|', cell_height);
        if (j > 0 && path[i][j - 1] && !maze->vertical_walls[i][j - 1])
          mvhline(y + cell_height / 2, x - cell_width / 2, '-', cell_width + 1);
        attroff(COLOR_PAIR(1));
      }
      if (maze->horizontal_walls[i][j]) {
        mvhline(y + cell_height, x + 1, '_', cell_width);
      }

      if (maze->vertical_walls[i][j]) {
        for (int k = 1; k <= cell_height; k++) {
          mvprintw(y + k, x + cell_width, "|");
        }
      }
    }
  }
}

bool get_dimension(int start_y, int start_x, int *rows, int *cols) {
  bool is_correct = true;

  attron(COLOR_PAIR(1));

  echo();
  mvprintw(start_y + 5, start_x, "Enter maze rows (1-50):");
  refresh();
  scanw("%d", rows);
  mvprintw(start_y + 6, start_x, "Enter maze cols (1-50):");
  refresh();
  scanw("%d", cols);
  noecho();

  attroff(COLOR_PAIR(1));

  if (*rows < 1 || *rows > 50 || *cols < 1 || *cols > 50) {
    *rows = 0;
    *cols = 0;
    is_correct = false;
  }

  return is_correct;
}

void print_menu(int start_y, int start_x) {
  attron(COLOR_PAIR(1));

  mvprintw(start_y, start_x, "f [load maze from file]");
  mvprintw(start_y + 1, start_x, "g [generate maze]");
  mvprintw(start_y + 2, start_x, "q [quit]");
  // mvprintw(start_y + 3, start_x, "p [show path]");
  // mvprintw(start_y + 4, start_x, "s [input start and end (if loaded)]");
  // mvprintw(start_y + 5, start_x, "start: %d %d", s->x, s->y);
  // mvprintw(start_y + 6, start_x, "end: %d %d", e->x, e->y);
  // mvprintw(start_y + 7, start_x, "t [train model]");
  // mvprintw(start_y + 8, start_x, "m [show ml path]");
  // mvprintw(start_y + 9, start_x, "c [load cave from file]");
  // mvprintw(start_y + 10, start_x + 2, "n [next step]");
  // mvprintw(start_y + 11, start_x + 2, "q [quit step-by-step mode]");

  attroff(COLOR_PAIR(1));
}

void main_menu() {
  Maze *maze = create_maze();

  bool quit = false;
  char filepath[BUFFER];
  int ch = 0;

  while (!quit) {
    print_menu(MENU_Y, MENU_X);

    ch = getch();

    switch (ch) {
      case 'q':
        quit = true;
        break;
      case 'f':
        f_case(maze, filepath);
        break;
      case 'g':
        g_case(maze);
        break;
      default:
        attron(COLOR_PAIR(3));
        mvprintw(2, 25, "Incorrect input, please try again\n");
        attron(COLOR_PAIR(3));
        break;
    }
  }
}

void get_filename(int start_y, int start_x, char *filepath) {
  char filename[BUFFER];

  attron(COLOR_PAIR(1));
  mvprintw(start_y + 5, start_x, "Enter the filename: ");
  refresh();
  echo();

  mvgetstr(start_y + 6, start_x, filename);

  snprintf(filepath, BUFFER * 2, DIR "%s", filename);

  noecho();
  clear();
  attroff(COLOR_PAIR(1));
}

void f_case(Maze *maze, char *filepath) {
  get_filename(MENU_Y, MENU_X, filepath);

  if (load_maze(filepath, maze)) {
    render_maze(0, 0, maze, 0, 0);
  } else {
    attron(COLOR_PAIR(2));
    mvprintw(2, 25, "Failed to load maze\n");
    attron(COLOR_PAIR(2));
  }
}

void g_case(Maze *maze) {
  int g_rows = 0;
  int g_cols = 0;

  bool is_correct_maze = get_dimension(MENU_Y, MENU_X, &g_rows, &g_cols);

  clear();

  print_menu(MENU_Y, MENU_X);

  if (is_correct_maze) {
    generate_maze(maze, g_rows, g_cols);

    render_maze(0, 0, maze, 0, 0);

    save_maze(maze, G_CASE_FILE);

  } else {
    attron(COLOR_PAIR(2));
    mvprintw(2, 25, "Incorrect dimension\n");
    attroff(COLOR_PAIR(2));
  }
}

int main(void) {
  // FILE *file = fopen("data.txt", "w");

  // fprintf(file, "%d\n", *g_rows);
  // fprintf(file, "%d\n", *g_cols);

  // fclose(file);

  if (initscr() == NULL) {
    fprintf(stderr, "Failed to initialize ncurses\n");
    return EXIT_FAILURE;
  }

  noecho();
  curs_set(0);
  keypad(stdscr, TRUE);

  start_color();
  init_pair(1, COLOR_GREEN, COLOR_BLACK);
  init_pair(2, COLOR_RED, COLOR_BLACK);
  init_pair(3, COLOR_YELLOW, COLOR_BLACK);

  main_menu();

  endwin();

  return EXIT_SUCCESS;
}