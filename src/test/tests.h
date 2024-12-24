#ifndef TESTS_H_
#define TESTS_H_

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

#endif
