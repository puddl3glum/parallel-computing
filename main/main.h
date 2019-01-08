#pragma once

typedef struct {
  bool aliveadj;
  bool alive;
} cell_t;

typedef struct {
  const uint64_t n;
  const uint64_t m;
  cell_t** const cells;
} board_t;

void freeboard(board_t);
board_t randomboard(uint64_t, uint64_t);
