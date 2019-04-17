/*
 * William Setzer
 * wjsetzer
 * CS 732
 * Homework 1
 * */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>

#include "game.h"

int check_neighbors(board_t, const uint64_t, const uint64_t);
void copy_board(board_t, board_t);
bool arraycomp(uint64_t*, uint64_t*, uint64_t);

board_t new_board(const uint64_t height, const uint64_t width) {
  /*
   * Return a 1-dimensional array (N+2)x(M+2) long with random values.
   * The values are initialized from 1..N and 1..M.
   * The edges are treated as ghost cells.
   * */

  // First and last N+2 are false.
  // For every chunk, the first and last are false.
  //
  // bool** cells = calloc(height + 2, sizeof(bool*));
  bool** cells = calloc(height + 2, sizeof(bool*));

  size_t y = 0;
  for (y = 0; y < height+2; y++) {
    bool* rowptr = calloc(width+2, sizeof(bool));
    cells[y] = rowptr;
  }
  
  board_t board = {height, width, cells};

  return board; 

}

board_t random_board(const uint64_t height, const uint64_t width, const double chance, const size_t seed) {
  /*
   * Return a 1-dimensional array (N+2)x(M+2) long with random values.
   * The values are initialized from 1..N and 1..M.
   * The edges are treated as ghost cells.
   * */

  // First and last N+2 are false.
  // For every chunk, the first and last are false.
  
  // srand((uint32_t)time(NULL));
  srand(seed);

  board_t board = new_board(height, width);

  size_t y = 1;
  for (y = 1; y <= board.height; y++) {
    size_t x = 1;
    for (x = 1; x <= board.width; x++) {
      int randomnum = rand();
      bool val = chance > ((double) randomnum / (double) RAND_MAX);
      if (val) {
        board.cells[y][x] = true;
        // not necessarily true, but ensure the cell will be
        // examined in the first iteration
        // count_neighbors(board, y, x);
      }
    }
  }


  return board; 
}

cyclesum_t newcyclesum(const uint64_t row, const uint64_t col, uint64_t maxcycles) {
  /*
   * Return a new cyclesum_t struct
   * */

  maxcycles = maxcycles + 1; // add 1 to make space for the cycle being summed

  // create an array for holding sums of length maxcycle
  uint64_t** const sums = calloc(sizeof(uint64_t*), maxcycles);

  size_t x = 0;
  for (x = 0; x < maxcycles; x++) {

    // create an array for holding row sums of length col
    sums[x] = calloc(sizeof(uint64_t), col);

  }

  cyclesum_t cyclesum = {
    maxcycles,
    col,
    row,
    0, // current cycle is currently 0
    sums
  };

  return cyclesum;
}

void freecyclesum(cyclesum_t cyclesum) {
  size_t x = 0;
  for (x = 0; x < cyclesum.maxcycle; x++) {
    free(cyclesum.sums[x]);
  }

  free(cyclesum.sums);
}

bool arraycomp(uint64_t* a, uint64_t* b, uint64_t len) {

  size_t x = 0;
  for (x = 0; x < len; x++) {
    if (a[x] != b[x]) return false;
  }

  return true;
}

bool checkcycles(cyclesum_t* cyclesum, board_t board) {
  /* Returns true if two cycles are equal, else false
   * Sets the current state of the board to be the next
   * cycle, replacing the oldest cycle
   * */

  // sum the alive cells in the rows of the board
  
  size_t y = 1;
  for (y = 1; y <= board.height; y++ ) {
    uint64_t sum = 0;

    size_t x = 1;
    for (x = 1; x <= board.width; x++) {
      if (board.cells[y][x]) {
        sum++;
      }
    }

    // set sum to cyclesum current
    cyclesum->sums[cyclesum->current][y - 1] = sum;
  }

  // compare current to other cycles
  for (y = 0; y < cyclesum->maxcycle; y++) {
    uint64_t currentcycle = cyclesum->current;

    if (y != currentcycle) {
      if ( arraycomp(cyclesum->sums[y], cyclesum->sums[currentcycle], cyclesum->row)) {
        return true;
      }
    }
  }

  // increment cycle
  // NOTE: there is no need to increment if two cycles matched
  cyclesum->current = (cyclesum->current + 1) % cyclesum->maxcycle;

  return false;
}


int check_neighbors(board_t board, const uint64_t y_pos, const uint64_t x_pos) {
  /*
   * increments the neighbors field of the surrounding neighbor
   * cells.
   * */

  size_t count = 0;

  int y = -1;
  for (y = -1; y <= 1; y++) {
    int x = -1;
    for (x = -1; x <= 1; x++) {

      if (x == 0 && y == 0) {
        continue;
      }

      if (board.cells[y + y_pos][x + x_pos]) {
        count++;
      }
    }
  }

  return count;
}

void advance_board(board_t current_gen, board_t next_gen) {

  // create temp board

  // copy old board to new board

  // # pragma omp parallel for
  size_t y = 1;
  for (y = 1; y <= current_gen.height; y++) {
    size_t x = 1;
    for (x = 1; x <= current_gen.width; x++) {
      
      bool current_cell = current_gen.cells[y][x];
      
      int count = check_neighbors(current_gen, y, x);

      if (current_cell && ( count > 3 || count < 2)) {
        current_cell = false;
      }
      else if (! current_cell && count == 3) {
        current_cell = true;
      }

      next_gen.cells[y][x] = current_cell;

    }
  }
}

void copy_board(board_t dest, board_t src) {
  size_t y = 0;
  for (y = 0; y < src.height + 2; y++) {
    memcpy(dest.cells[y], src.cells[y], sizeof(bool) * src.height + 2);
  }
}

void printboard(board_t board) {
  size_t y = 1;
  for (y = 1; y <= board.height; y++ ){
    size_t x = 1;
    for (x = 1; x <= board.width; x++) {
      printf(board.cells[y][x] ? "█" : "░");
    }
    puts("");
  }
}

void printfullboard(board_t board) {
  size_t y = 0;
  for (y = 0; y < board.height + 2; y++ ){
    size_t x = 0;
    for (x = 0; x < board.width + 2; x++) {
      printf(board.cells[x][y] ? "█" : "░");
    }
    puts("");
  }
}

void free_board(board_t board) {
  size_t y = 0;
  for (y = 0; y < board.height + 2; y++) {
    free(board.cells[y]);
  }

  free(board.cells);
}

