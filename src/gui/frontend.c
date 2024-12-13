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

void main_menu() {

  printw("Fron main menu\n");

}


int main(void) {
    
    if (initscr() == NULL) {
        fprintf(stderr, "Ошибка инициализации ncurses\n");
        return EXIT_FAILURE;
    }

    // Отключаем отображение вводимых символов
    noecho();
    // Скрываем курсор
    curs_set(0);
    // Включаем поддержку клавиш
    keypad(stdscr, TRUE);

    // Начинаем цветовую работу
    start_color();
    init_pair(1, COLOR_GREEN, COLOR_BLACK); // Создаем пару цветов

    Maze *maze = create_maze();

    if (load_maze("examples/ex_maze.txt", maze) == 1) {
        render_maze(0, 0, 22, 76, maze, 0, 0);
    } else {
        printw("Failed to load maze\n");
    }

    refresh(); // Обновляем экран

    getch(); // Ожидаем ввода

    // Завершаем работу ncurses
    endwin();

    return EXIT_SUCCESS;
    
}