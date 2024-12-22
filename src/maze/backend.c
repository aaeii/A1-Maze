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

bool load_maze(char *filepath, Maze *maze) {
  bool isLoad = false;
  FILE *file = fopen(filepath, "r");
  if (file != NULL && fscanf(file, "%d %d", &maze->rows, &maze->cols) == 2) {
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

void save_maze(Maze *maze, char *filename) {
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

Cave *create_cave() {
  Cave *cave = calloc(1, sizeof(Cave));

  if (cave == NULL) {
    return NULL;
  }

  cave->rows = 0;
  cave->cols = 0;
  memset(cave->cells, 0, sizeof(cave->cells));
  return cave;
}

void free_cave(Cave *cave) { free(cave); }

int **create_path() {
  int **path = calloc(MAX_SIZE, sizeof(int *));

  if (path == NULL) {
    return NULL;
  }

  for (int i = 0; i < MAX_SIZE; i++) {
    path[i] = calloc(MAX_SIZE, sizeof(int));

    // if (path[i] == NULL) {
    //   for (int j = 0; j < i; j++) {
    //     free(path[j]);
    //   }
    //   free(path);
    //   return NULL;
    // }
  }
  return path;
}

void free_path(int **path) {
  for (int i = 0; i < MAX_SIZE; ++i) {
    free(path[i]);
  }
  free(path);
}

bool load_cave(char *filename, Cave *cave) {
  bool isLoad = false;
  FILE *file = fopen(filename, "r");

  if (file != NULL && fscanf(file, "%d %d", &cave->rows, &cave->cols) == 2) {
    for (int i = 0; i < cave->rows; ++i) {
      for (int j = 0; j < cave->cols; ++j) {
        fscanf(file, "%d", &cave->cells[i][j]);
      }
    }
    isLoad = true;

    fclose(file);
  }
  return isLoad;
}

void initialize_can_change(int can_change[MAX_SIZE][MAX_SIZE], int rows,
                           int cols, int initial_chance) {
  srand(time(NULL));
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      int random_value = rand() % 100;
      if (random_value < initial_chance) {
        can_change[i][j] = 1;
      } else {
        can_change[i][j] = 0;
      }
    }
  }
}

int count_live_neighbors(Cave *cave, int y, int x) {
  int count = 0, ny, nx;
  int directions[8][2] = {{-1, -1}, {-1, 0}, {-1, 1}, {0, -1},
                          {0, 1},   {1, -1}, {1, 0},  {1, 1}};
  for (int i = 0; i < 8; i++) {
    ny = y + directions[i][0];
    nx = x + directions[i][1];
    if (ny >= 0 && ny < cave->rows && nx >= 0 && nx < cave->cols) {
      if (cave->cells[ny][nx] == 1) {
        count++;
      }
    } else {
      count++;
    }
  }
  return count;
}

void update_cave(Cave *cave, int birth_limit, int death_limit,
                 int can_change[MAX_SIZE][MAX_SIZE]) {
  int new_cells[MAX_SIZE][MAX_SIZE];
  int live_neighbors = 0;

  for (int i = 0; i < cave->rows; i++) {
    for (int j = 0; j < cave->cols; j++) {
      new_cells[i][j] = cave->cells[i][j];
    }
  }

  for (int i = 0; i < cave->rows; i++) {
    for (int j = 0; j < cave->cols; j++) {
      live_neighbors = count_live_neighbors(cave, i, j);
      if (cave->cells[i][j] == 1 && live_neighbors < death_limit &&
          can_change[i][j]) {
        new_cells[i][j] = 0;
      } else if (cave->cells[i][j] == 0 && live_neighbors > birth_limit &&
                 can_change[i][j]) {
        new_cells[i][j] = 1;
      }
    }
  }

  for (int i = 0; i < cave->rows; i++) {
    for (int j = 0; j < cave->cols; j++) {
      cave->cells[i][j] = new_cells[i][j];
    }
  }
}

int get_max_size() { return MAX_SIZE; }

void load_maze_from_content(char *str, Maze *maze) {
  char *ptr = str;
  int read = 0;
  sscanf(ptr, "%d %d%n", &maze->rows, &maze->cols, &read);
  ptr += read;
  for (int i = 0; i < maze->rows; ++i) {
    for (int j = 0; j < maze->cols; ++j) {
      sscanf(ptr, "%d%n", &maze->vertical_walls[i][j], &read);
      ptr += read;
    }
  }
  for (int i = 0; i < maze->rows; ++i) {
    for (int j = 0; j < maze->cols; ++j) {
      sscanf(ptr, "%d%n", &maze->horizontal_walls[i][j], &read);
      ptr += read;
    }
  }
}

bool valid(Maze *m, int x, int y) {
  return (x >= 0 && x < m->rows && y >= 0 && y < m->cols);
}

int bfs(Maze *maze, Point start, Point end, int **path) {
  empty_arr(path);
  int is_path = 0;
  if (valid(maze, start.x, start.y) == 1 && valid(maze, end.x, end.y) == 1) {
    int visited_point[MAX_SIZE][MAX_SIZE] = {{-1}};
    Point queue[MAX_SIZE * MAX_SIZE];  // oчередь для хранения координат ячеек
    Point prev[MAX_SIZE][MAX_SIZE] = {{{-1, -1}}};  // предыдущие координаты
    int head = 0, tail = 0;
    queue[tail++] = start;
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
  }
  return is_path;
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
    if (valid(maze, new_x, new_y) &&
        check_presence_of_walls(i, maze, cur, new_x, new_y) == 0 &&
        v_p[new_x][new_y] == 0) {
      v_p[new_x][new_y] = 1;
      q[(*tail)++] = (Point){new_x, new_y};  // добавляем новую ячейку в очередь
      prev[new_x][new_y] =
          cur;  // текущая ячейка как родитель соседней ячейки new
    }
  }
}