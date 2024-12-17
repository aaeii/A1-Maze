#include "backend.h"

Maze *create_maze() {
  Maze *maze = calloc(1, sizeof(Maze));

  if (maze == NULL) {
    return NULL;
  }

  maze->rows = 0;
  maze->cols = 0;
  memset(maze->vertical_walls, 0, sizeof(maze->vertical_walls));
  memset(maze->horizontal_walls, 0, sizeof(maze->horizontal_walls));

  return maze;
}

void free_maze(Maze *maze) { free(maze); }

bool load_maze(const char *filepath, Maze *maze) {
  bool isLoad = false;
  FILE *file = fopen(filepath, "r");
  if (file != NULL) {
    fscanf(file, "%d %d", &maze->rows, &maze->cols);
    for (int i = 0; i < maze->rows; i++) {
      for (int j = 0; j < maze->cols; j++) {
        fscanf(file, "%d", &maze->vertical_walls[i][j]);
      }
    }
    for (int i = 0; i < maze->rows; i++) {
      for (int j = 0; j < maze->cols; j++) {
        fscanf(file, "%d", &maze->horizontal_walls[i][j]);
      }
    }
    fclose(file);
    isLoad = true;
  }
  return isLoad;
}

void merge_sets(Maze *maze, int i, int cols, int rows, int *sets) {
  int old_set = 0;
  for (int j = 0; j < cols - 1; j++) {
    if (sets[j] != sets[j + 1] && (rand() % 2 == 0 || i == rows - 1)) {
      maze->vertical_walls[i][j] = 0;
      old_set = sets[j + 1];
      for (int k = 0; k < cols; k++) {
        if (sets[k] == old_set) {
          sets[k] = sets[j];
        }
      }
    } else {
      maze->vertical_walls[i][j] = 1;
    }
  }
}

void generate_maze(Maze *maze, int rows, int cols) {
  // Устанавливает количество строк и столбцов в структуре Maze
  maze->rows = rows;
  maze->cols = cols;

  // Создает массив sets для хранения идентификаторов множеств (групп ячеек
  // лабиринта)
  int sets[MAX_SIZE];
  // Инициализирует генератор случайных чисел с помощью текущего времени
  int next_set = 0;
  srand(time(NULL));

  // Заполняет массив sets, присваивая каждой колонне уникальный идентификатор
  // множества
  for (int j = 0; j < cols; j++) {
    sets[j] = next_set++;
  }

  // Для каждой строки (i) вызывает функцию merge_sets (вертикальные стены),
  // которая объединяет множества в зависимости от текущих стен
  for (int i = 0; i < rows; i++) {
    merge_sets(maze, i, cols, rows, sets);
    // Создает set_connected для отслеживания, были ли соединены множества в
    // текущей строке
    bool set_connected[MAX_SIZE * MAX_SIZE] = {0};
    for (int j = 0; j < cols; j++) {
      // Если это последняя строка, устанавливает горизонтальные стены (границу)
      if (i == rows - 1) {
        maze->horizontal_walls[i][j] = 1;
        // Или убирает горизонтальную стену и помечает множество как соединенное
      } else if (rand() % 2 == 0 || !set_connected[sets[j]]) {
        maze->horizontal_walls[i][j] = 0;
        set_connected[sets[j]] = true;
      } else {
        // Если предыдущие условия не выполнены, оставляет стену на месте
        maze->horizontal_walls[i][j] = 1;
      }
    }

    // Если горизонтальная стена установлена,
    // обновляет идентификаторы множеств для следующей строки
    for (int j = 0; j < cols; j++) {
      if (maze->horizontal_walls[i][j] == 1 && i < rows - 1) {
        sets[j] = next_set++;
      }
      // Устанавливает вертикальные стены на правом краю лабиринта
      if (j == cols - 1) maze->vertical_walls[i][j] = 1;
    }
  }
}

void save_maze(const Maze *maze, char *filename) {
  FILE *file = fopen(filename, "w");
  if (file != NULL) {
    fprintf(file, "%d %d\n", maze->rows, maze->cols);

    for (int i = 0; i < maze->rows; ++i) {
      for (int j = 0; j < maze->cols; ++j) {
        fprintf(file, "%d ", maze->vertical_walls[i][j]);
      }
      fprintf(file, "\n");
    }
    fprintf(file, "\n");
    for (int i = 0; i < maze->rows; ++i) {
      for (int j = 0; j < maze->cols; ++j) {
        fprintf(file, "%d ", maze->horizontal_walls[i][j]);
      }
      fprintf(file, "\n");
    }
    fclose(file);
  }
}