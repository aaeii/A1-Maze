#include "frontend.h"
#include "../maze/backend.h"

void render_maze(int start_y, int start_x, int max_height, int max_width,
                 const Maze *maze, int **path, bool is_path) {
                  
  int cell_height = max_height / maze->rows;
  int cell_width = max_width / maze->cols;
  int x, y;

  mvhline(start_y, start_x, '_', max_width);
  mvvline(start_y + 1, start_x, '|', max_height);

  for (int i = 0; i < maze->rows; ++i) {
    for (int j = 0; j < maze->cols; ++j) {
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
        for (int k = 1; k <= cell_height; ++k) {
          mvprintw(y + k, x + cell_width, "|");
        }
      }
    }
  }
}

void print_menu(int start_y, int start_x) {

  attron(COLOR_PAIR(1));

  mvprintw(start_y, start_x, "f [load maze from file]");
  // mvprintw(start_y + 1, start_x, "g [generate maze]");
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

  int menu_start_y = 0;
  int menu_start_x = 80;
  bool quit = false;
  char filepath[256];

  int ch = 0;

  while (!quit) {

    print_menu(menu_start_y, menu_start_x);

    ch = getch();

    

    switch (ch) {
    case 'q':
      quit = true;
      break;
    case 'f':
      f_case(maze, filepath);  
    default:
      break;
    }

  }

}

void get_filename(int start_y, int start_x, char *filepath) {

  char filename[256];

  attron(COLOR_PAIR(1));
  mvprintw(start_y + 5, start_x, "Enter the filename: ");
  refresh();
  echo();

  mvgetstr(start_y + 6, start_x, filename);

  snprintf(filepath, 512, DIR"%s", filename);

  noecho();
  clear();
  attroff(COLOR_PAIR(1));
}

void f_case(Maze *maze, char *filepath) {

  get_filename(0, 80, filepath);

  if (load_maze(filepath, maze)) {
        render_maze(0, 0, 22, 76, maze, 0, 0);
  } else {
      attron(COLOR_PAIR(2));
        printw("Failed to load maze\n");
        attron(COLOR_PAIR(2));
  }
}


int main(void) {
    
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

    main_menu();


    endwin();

    return EXIT_SUCCESS;
    
}