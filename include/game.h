#pragma once

#include <stdbool.h>
#include <stdint.h>

typedef struct {
  bool alive;
} cell_t;

typedef struct {
  uint64_t width;
  uint64_t height;
  cell_t* cells;
} board_t;

typedef struct {
  int32_t threads;
  int32_t rank;
} game_t;

void step_generation(board_t, board_t);

void freecells(board_t);

board_t randomboard(uint64_t, uint64_t, uint32_t);
board_t blankboard(uint64_t, uint64_t);
void copyboard(game_t, board_t, board_t);

void printfullboard(board_t);
void printboard(board_t);


