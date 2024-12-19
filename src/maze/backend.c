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

bool valid(Maze *m, int x, int y) {
  return (x >= 0 && x < m->rows && y >= 0 && y < m->cols);
}
bool check_presence_of_walls(int i, Maze *m, Point current, int new_x,
                             int new_y) {
  return ((i == 1 && m->horizontal_walls[current.x][current.y]) ||
          (i == 0 && m->vertical_walls[current.x][current.y]) ||
          (i == 3 && m->horizontal_walls[new_x][new_y]) ||
          (i == 2 && m->vertical_walls[new_x][new_y]));
}
void empty_arr(int **a) {
  for (int i = 0; i < MAX_SIZE; i++) {
    for (int j = 0; j < MAX_SIZE; j++) {
      a[i][j] = 0;
    }
  }
}
void check_neighboring_cells(Maze *maze, Point cur, int v_p[][MAX_SIZE],
                             Point q[MAX_SIZE * MAX_SIZE],
                             Point prev[][MAX_SIZE], int *tail) {
  int directions[4][2] = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};
  for (int i = 0; i < 4; i++) {
    int new_x = cur.x + directions[i][0], new_y = cur.y + directions[i][1];
    if (valid(maze, new_x, new_y) && v_p[new_x][new_y] == 0 &&
        !check_presence_of_walls(i, maze, cur, new_x, new_y)) {
      v_p[new_x][new_y] = 1;
      q[(*tail)++] = (Point){new_x, new_y};  // добавляем новую ячейку в очередь
      prev[new_x][new_y] =
          cur;  //текущая ячейка как родитель соседней ячейки new
    }
  }
}
int bfs(Maze *maze, Point start, Point end, int **path) {
  empty_arr(path);
  int is_path = 0;
  int visited_point[MAX_SIZE][MAX_SIZE] = {0};
  Point queue[MAX_SIZE * MAX_SIZE];  // oчередь для хранения координат ячеек
  Point prev[MAX_SIZE][MAX_SIZE] = {{{-1, -1}}};  // предыдущие координаты
  int head = 0, tail = 1;
  queue[tail] = start;
  visited_point[start.x][start.y] = 1;
  while ((head < tail) || is_path != 1) {
    Point current = queue[head++];  // берем текущую точку из очереди
    if (current.y == end.y && current.x == end.x) {  //восст путь
      Point current_path = end;
      while (current_path.y != -1) {
        path[current_path.x][current_path.y] = 1;
        current_path = prev[current_path.x][current_path.y];
      }
      is_path = 1;
    }
    check_neighboring_cells(maze, current, visited_point, queue, prev, &tail);
  }
  return is_path;
}

void free_path(int **path) {
  if (path) {
    for (int i = 0; i < MAX_SIZE; ++i) {
      free(path[i]);
    }
    free(path);
  }
}