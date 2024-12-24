#include "tests.h"

START_TEST(test_create_cave) {
  Cave *cave = create_cave();

  ck_assert_ptr_nonnull(cave);
  ck_assert_int_eq(cave->rows, 0);
  ck_assert_int_eq(cave->cols, 0);

  for (int i = 0; i < MAX_SIZE; i++) {
    for (int j = 0; j < MAX_SIZE; j++) {
      ck_assert_int_eq(cave->cells[i][j], 0);
    }
  }

  free_cave(cave);
}
END_TEST

START_TEST(test_create_maze) {
  Maze *maze = create_maze();
  ck_assert_ptr_nonnull(maze);
  ck_assert_int_eq(maze->rows, 0);
  ck_assert_int_eq(maze->cols, 0);
  for (int i = 0; i < MAX_SIZE; i++) {
    for (int j = 0; j < MAX_SIZE; j++) {
      ck_assert_int_eq(maze->vertical_walls[i][j], 0);
      ck_assert_int_eq(maze->horizontal_walls[i][j], 0);
    }
  }
  free_maze(maze);
}
END_TEST

START_TEST(test_load_maze) {
  Maze *maze = create_maze();

  FILE *file = fopen("test_maze.txt", "w");
  fprintf(file, "5 5\n");
  for (int i = 0; i < 5; i++) {
    for (int j = 0; j < 5; j++) {
      fprintf(file, "0 ");
    }
    fprintf(file, "\n");
  }
  for (int i = 0; i < 5; i++) {
    for (int j = 0; j < 5; j++) {
      fprintf(file, "0 ");
    }
    fprintf(file, "\n");
  }
  fclose(file);

  bool result = load_maze("test_maze.txt", maze);
  ck_assert(result);
  ck_assert_int_eq(maze->rows, 5);
  ck_assert_int_eq(maze->cols, 5);
  for (int i = 0; i < maze->rows; i++) {
    for (int j = 0; j < maze->cols; j++) {
      ck_assert_int_eq(maze->vertical_walls[i][j], 0);
      ck_assert_int_eq(maze->horizontal_walls[i][j], 0);
    }
  }
  remove("test_maze.txt");
  free_maze(maze);
}
END_TEST

START_TEST(test_save_maze) {
  Maze *maze = create_maze();
  maze->rows = 5;
  maze->cols = 5;
  save_maze(maze, "output_maze.txt");

  Maze *loaded_maze = create_maze();
  bool result = load_maze("output_maze.txt", loaded_maze);
  ck_assert(result);
  ck_assert_int_eq(loaded_maze->rows, 5);
  ck_assert_int_eq(loaded_maze->cols, 5);
  free_maze(maze);
  free_maze(loaded_maze);
  remove("output_maze.txt");
}
END_TEST

START_TEST(test_generate_maze) {
  Maze *maze = create_maze();
  generate_maze(maze, TEST_MAZE_ROWS, TEST_MAZE_COLS);
  ck_assert_int_eq(maze->rows, TEST_MAZE_ROWS);
  ck_assert_int_eq(maze->cols, TEST_MAZE_COLS);
  for (int i = 0; i < TEST_MAZE_ROWS; i++) {
    for (int j = 0; j < TEST_MAZE_COLS; j++) {
      ck_assert_int_le(maze->vertical_walls[i][j], 1);
      ck_assert_int_le(maze->horizontal_walls[i][j], 1);
    }
  }
  free_maze(maze);
}
END_TEST

START_TEST(load_maze_) {
  Maze *maze = create_maze();
  FILE *f = fopen("examples/maze4.txt", "r");
  char str[200];
  int i = 0;
  if (f != NULL) {
    char ch;
    do {
      ch = fgetc(f);
      str[i] = ch;
      i++;
    } while (ch != EOF);
    fclose(f);
  }
  str[i] = '\0';
  load_maze_from_content(str, maze);
  ck_assert_int_eq(maze->cols, 4);
  ck_assert_int_eq(maze->rows, 4);
  free_maze(maze);
}
END_TEST

START_TEST(max) { ck_assert_int_eq(get_max_size(), MAX_SIZE); }

START_TEST(test_initialize_can_change) {
  int can_change[MAX_SIZE][MAX_SIZE];
  initialize_can_change(can_change, TEST_CAVE_ROWS, TEST_CAVE_COLS, 50);
  for (int i = 0; i < TEST_CAVE_ROWS; i++) {
    for (int j = 0; j < TEST_CAVE_COLS; j++) {
      ck_assert_int_le(can_change[i][j], 1);
    }
  }
}
END_TEST

START_TEST(test_count_live_neighbors) {
  Cave cave;
  cave.rows = 3;
  cave.cols = 3;
  int cells[3][3] = {{1, 0, 1}, {0, 1, 0}, {1, 0, 1}};
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      cave.cells[i][j] = cells[i][j];
    }
  }

  int live_neighbors = count_live_neighbors(&cave, 1, 1);
  ck_assert_int_eq(live_neighbors, 4);
  live_neighbors = count_live_neighbors(&cave, 0, 0);
  ck_assert_int_eq(live_neighbors, 6);
}
END_TEST

START_TEST(test_update_cave) {
  Cave cave;
  int can_change[MAX_SIZE][MAX_SIZE];
  cave.rows = 3;
  cave.cols = 3;
  int cells[3][3] = {{1, 0, 1}, {0, 1, 0}, {1, 0, 1}};
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      cave.cells[i][j] = cells[i][j];
    }
  }
  for (int i = 0; i < cave.rows; i++) {
    for (int j = 0; j < cave.cols; j++) {
      can_change[i][j] = 1;
    }
  }
  update_cave(&cave, 7, 2, can_change);
  ck_assert_int_eq(cave.cells[1][1], 1);
  ck_assert_int_eq(cave.cells[0][1], 0);
  update_cave(&cave, 1, 7, can_change);
  ck_assert_int_eq(cave.cells[2][2], 0);
}
END_TEST

START_TEST(test_load_cave) {
  Cave cave;
  FILE *file = fopen("test_cave.txt", "w");
  fprintf(file, "3 3\n");
  fprintf(file, "1 0 1\n");
  fprintf(file, "0 1 0\n");
  fprintf(file, "1 0 1\n");
  fclose(file);

  bool result = load_cave("test_cave.txt", &cave);
  ck_assert(result);
  ck_assert_int_eq(cave.rows, 3);
  ck_assert_int_eq(cave.cols, 3);
  int expected_cells[3][3] = {{1, 0, 1}, {0, 1, 0}, {1, 0, 1}};
  for (int i = 0; i < cave.rows; ++i) {
    for (int j = 0; j < cave.cols; ++j) {
      ck_assert_int_eq(cave.cells[i][j], expected_cells[i][j]);
    }
  }
  remove("test_cave.txt");
}

START_TEST(test_bfs) {
  Maze *maze = create_maze();
  int **path = create_path();
  load_maze("examples/maze2.txt", maze);
  ck_assert_int_eq(maze->rows, 10);
  ck_assert_int_eq(maze->cols, 10);
  Point start = {0, 0};
  Point end = {2, 4};
  ck_assert_int_eq(bfs(maze, start, end, path), 1);

  start.x = 12;
  end.y = -1;
  ck_assert_int_eq(bfs(maze, start, end, path), 0);

  free_maze(maze);
  free_path(path);
}

int main(void) {
  srand(time(NULL));
  Suite *s = suite_create("Maze");
  TCase *tc_core = tcase_create("Core");

  tcase_add_test(tc_core, test_create_maze);
  tcase_add_test(tc_core, test_create_cave);
  tcase_add_test(tc_core, test_load_maze);
  tcase_add_test(tc_core, test_save_maze);
  tcase_add_test(tc_core, test_generate_maze);
  tcase_add_test(tc_core, load_maze_);
  tcase_add_test(tc_core, max);
  tcase_add_test(tc_core, test_initialize_can_change);
  tcase_add_test(tc_core, test_count_live_neighbors);
  tcase_add_test(tc_core, test_update_cave);
  tcase_add_test(tc_core, test_load_cave);
  tcase_add_test(tc_core, test_bfs);

  suite_add_tcase(s, tc_core);

  SRunner *sr = srunner_create(s);
  srunner_set_fork_status(sr, CK_NOFORK);
  srunner_run_all(sr, CK_NORMAL);
  int number_failed = srunner_ntests_failed(sr);
  srunner_free(sr);

  return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
