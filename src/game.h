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

typedef struct {
  uint64_t maxcycle;
  uint64_t col;
  uint64_t row;
  uint64_t current; // which cycle is currently on
  uint64_t  ** const sums; // array of arrays of row sums
} cyclesum_t;

void freecells(board_t);
board_t randomboard(uint64_t, uint64_t);
board_t blankboard(uint64_t, uint64_t);
cyclesum_t newcyclesum(uint64_t, uint64_t, uint64_t);
void freecyclesum(cyclesum_t);
bool checkcycles(cyclesum_t*, board_t);
void printfullboard(board_t);
void printboard(board_t);
void simgen(board_t);
