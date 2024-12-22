#include <check.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "../maze/backend.h"

#define TEST_MAZE_ROWS 5
#define TEST_MAZE_COLS 5
#define TEST_MAX_MAZE_ROWS 50
#define TEST_MAX_MAZE_COLS 50
#define TEST_CAVE_ROWS 5
#define TEST_CAVE_COLS 5

typedef enum { VALID, INVALID_START, INVALID_END } point_valid;

typedef enum { UP, DOWN, LEFT, RIGHT, ACTIONS_COUNT } actions;

typedef enum { MOVE_VALID, MOVE_INVALID, MOVE_OUT } valid_move;

enum path { NO_PATH = -9999 };

// bool has_path(Maze *maze, Point *start, Point *end) {
//   int **path = create_path();
//   int distance = get_solution_bfs(maze, start, end, path);
//   bool result = (distance != NO_PATH);
//   free_path(path);
//   return result;
// }

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

// START_TEST(test_perfect_maze_generation) {
//   Maze maze;
//   Point start, end;
//   generate_maze(&maze, TEST_MAX_MAZE_ROWS, TEST_MAX_MAZE_COLS);

//   int num_tests = 10000;
//   bool all_paths_exist = true;
//   srand(time(NULL));

//   for (int test = 0; test < num_tests; ++test) {
//     start.x = rand() % maze.rows;
//     start.y = rand() % maze.cols;
//     end.x = rand() % maze.rows;
//     end.y = rand() % maze.cols;
//     if (!has_path(&maze, &start, &end) && all_paths_exist) {
//       all_paths_exist = false;
//     }
//   }
//   ck_assert_msg(all_paths_exist,
//                 "Not all randomly chosen pairs of points are connected.");
// }
// END_TEST

// START_TEST(ml_test) {
//   Maze *maze = create_maze();
//   int **path = create_path();
//   Point start = {0, 0};
//   Point end = {3, 3};
//   float ***q_table = create_q_table();
//   initialize_q_table(q_table);
//   load_maze("test.txt", maze);
//   train(&start, &end, q_table, maze, false);
//   int p = get_solution_bfs(maze, &start, &end, path);
//   ck_assert_int_eq(p, 8);
//   int **path2 = create_path();
//   find_path_with_q_table(&start, &end, path2, q_table);
//   ck_assert_int_eq(path[end.x][end.y], path2[end.x][end.y]);
//   save_q_table("test_save", q_table);
//   FILE *f = fopen("test_save", "r");
//   ck_assert_ptr_nonnull(f);
//   if (f != NULL) {
//     fclose(f);
//     remove("test_save");
//   }
//   free_q_table(q_table);
//   free_maze(maze);
//   free_path(path);
//   free_path(path2);
// }
// END_TEST

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

// START_TEST(points) {
//   Maze *maze = create_maze();
//   Point start = {0, 0};
//   Point end = {3, 4};
//   load_maze("test.txt", maze);
//   ck_assert_int_eq(areStartEndValid(maze, &start, &end), INVALID_END);
//   start.x = 40;
//   end.y = 3;
//   ck_assert_int_eq(areStartEndValid(maze, &start, &end), INVALID_START);
//   start.x = 0;
//   ck_assert_int_eq(areStartEndValid(maze, &start, &end), VALID);
//   free_maze(maze);
// }
// END_TEST

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
  tcase_add_test(tc_core, test_load_maze);
  tcase_add_test(tc_core, test_save_maze);
  tcase_add_test(tc_core, test_generate_maze);
  // tcase_add_test(tc_core, test_perfect_maze_generation);
  // tcase_add_test(tc_core, ml_test);
  tcase_add_test(tc_core, load_maze_);
  // tcase_add_test(tc_core, points);
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
