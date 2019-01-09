#pragma once

#include <stdbool.h>
#include <stdint.h>

typedef struct {
  int_fast8_t neighbors;
  bool alive;
} cell_t;

typedef struct {
  uint64_t row;
  uint64_t col;
  cell_t** cells;
} board_t;

void freecells(board_t);
board_t randomboard(uint64_t, uint64_t);
board_t blankboard(uint64_t, uint64_t);
void printfullboard(board_t);
void printboard(board_t);
void simgen(board_t);
