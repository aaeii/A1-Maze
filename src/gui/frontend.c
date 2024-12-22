#include "frontend.h"

#include "../maze/backend.h"

void render_maze(int start_y, int start_x, Maze *maze, int **path,
                 int is_path) {
  int cell_height = MAX_HEIGHT / maze->rows,
      cell_width = MAX_WIDTH / maze->cols, x = 0, y = 0;
  mvhline(start_y, start_x, '_', MAX_WIDTH);
  mvvline(start_y + 1, start_x, '|', MAX_HEIGHT);
  for (int i = 0; i < maze->rows; i++) {
    for (int j = 0; j < maze->cols; j++) {
      y = start_y + i * cell_height;
      x = start_x + j * cell_width;
      if (path[i][j] && is_path == 1) {
        attron(COLOR_PAIR(1));
        int center_h = cell_height / 2, center_w = cell_width / 2;
        if (i > 0 && path[i - 1][j] && !maze->horizontal_walls[i - 1][j])
          mvvline(y - center_h, x + center_w, '|', cell_height);
        if (j > 0 && path[i][j - 1] && !maze->vertical_walls[i][j - 1])
          mvhline(y + center_h, x - center_w, '-', cell_width);
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
  mvprintw(start_y, start_x, "f - load maze from file");
  mvprintw(start_y + 1, start_x, "g - generate maze");
  mvprintw(start_y + 2, start_x, "p - show path");
  mvprintw(start_y + 3, start_x, "q - quit");
  attroff(COLOR_PAIR(1));
}

void main_menu() {
  Maze *maze = create_maze();
  int **path = calloc(MAX_SIZE, sizeof(int *));
  for (int i = 0; i < MAX_SIZE; ++i) {
    path[i] = calloc(MAX_SIZE, sizeof(int));
  }
  bool quit = false;
  char filepath[BUFFER];
  int ch = 0;
  Point start;
  Point end;

  while (!quit) {
    print_menu(MENU_Y, MENU_X);
    ch = getch();
    switch (ch) {
      case 'q':
        quit = true;
        break;
      case 'f':
        f_case(maze, filepath, path);
        break;
      case 'g':
        g_case(maze, path);
        break;
      case 'p':
        p_case(start, end, maze, path);
        break;
      default:
        attron(COLOR_PAIR(3));
        mvprintw(2, 25, "Incorrect input, please try again\n");
        attron(COLOR_PAIR(3));
        break;
    }
  }
  free_path(path);
}

void p_case(Point start, Point end, Maze *maze, int **path) {
  attron(COLOR_PAIR(1));
  mvprintw(MENU_Y + 5, MENU_X, "Start point: ");
  refresh();
  echo();
  scanw("%d%d", &start.x, &start.y);
  mvprintw(MENU_Y + 6, MENU_X, "End point: ");
  scanw("%d%d", &end.x, &end.y);
  noecho();
  clear();
  attroff(COLOR_PAIR(1));

  if (valid(maze, start.x, start.y) == 1) {
    int path_check = bfs(maze, start, end, path);
    if (path_check == 1) {
      render_maze(0, 0, maze, path, 1);
    } else {
      mvprintw(MENU_Y + 5, MENU_X, "Path not found\n");
      clear();
    }
  } else {
    mvprintw(MENU_Y + 5, MENU_X, "Incorrect point\n");
    clear();
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

void f_case(Maze *maze, char *filepath, int **path) {
  get_filename(MENU_Y, MENU_X, filepath);
  if (load_maze(filepath, maze)) {
    render_maze(0, 0, maze, path, 0);
  } else {
    attron(COLOR_PAIR(2));
    mvprintw(2, 25, "Failed to load maze\n");
    attron(COLOR_PAIR(2));
  }
}

void g_case(Maze *maze, int **path) {
  int g_rows = 0;
  int g_cols = 0;

  bool is_correct_maze = get_dimension(MENU_Y, MENU_X, &g_rows, &g_cols);

  clear();

  print_menu(MENU_Y, MENU_X);

  if (is_correct_maze) {
    generate_maze(maze, g_rows, g_cols);
    render_maze(0, 0, maze, path, 0);
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