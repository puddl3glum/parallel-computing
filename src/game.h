#pragma once

#include <stdbool.h>
#include <stdint.h>

typedef struct {
  bool aliveadj;
  bool alive;
} cell_t;

typedef struct {
  const uint64_t row;
  const uint64_t col;
  cell_t** const cells;
} board_t;

void freeboard(board_t);
board_t randomboard(uint64_t, uint64_t);
void printboard(board_t);
void simgen(board_t);
