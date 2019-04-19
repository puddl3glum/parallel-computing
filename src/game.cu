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

#include "game.cuh"

// void copy_board(board_t, board_t);
// bool arraycomp(uint64_t*, uint64_t*, uint64_t);
__device__
void check_neighbors(bool*, const uint64_t, const uint64_t);

bool* new_board(const uint64_t height, const uint64_t width) {
  /*
   * Return a 1-dimensional array (N+2)x(M+2) long with random values.
   * The values are initialized from 1..N and 1..M.
   * The edges are treated as ghost cells.
   * */

  // First and last N+2 are false.
  // For every chunk, the first and last are false.
  //
  // allocate contiguous memory
  bool* cells = (bool*) calloc((height + 2) * (width + 2), sizeof(bool));
  
  return cells; 

}

bool* random_board(const uint64_t height, const uint64_t width, const double chance, const uint64_t seed) {
  /*
   * Return a 1-dimensional array (N+2)x(M+2) long with random values.
   * The values are initialized from 1..N and 1..M.
   * The edges are treated as ghost cells.
   * */

  // First and last N+2 are false.
  // For every chunk, the first and last are false.
  
  // srand((uint32_t)time(NULL));
  srand(seed);

  bool* board = new_board(height, width);

  size_t y = 1;
  for (y = 1; y <= height; y++) {
    size_t x = 1;
    for (x = 1; x <= width; x++) {
      int randomnum = rand();
      bool val = chance > ((double) randomnum / (double) RAND_MAX);
      if (val) {
        board[y * height + x] = true;
        // not necessarily true, but ensure the cell will be
        // examined in the first iteration
        // count_neighbors(board, y, x);
      }
    }
  }


  return board; 
}

/*
cyclesum_t newcyclesum(const uint64_t row, const uint64_t col, uint64_t maxcycles) {

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
*/

/*
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
*/

/*
bool checkcycles(cyclesum_t* cyclesum, board_t board) {
   Returns true if two cycles are equal, else false
   * Sets the current state of the board to be the next
   * cycle, replacing the oldest cycle
   * 

  // sum the alive cells in the rows of the board
  
  size_t y = 1;
  for (y = 1; y <= board.height; y++ ) {
    uint64_t sum = 0;

    size_t x = 1;
    for (x = 1; x <= board.width; x++) {
      if (board.cells[y * board.height + x]) {
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
*/


__device__
uint_fast8_t check_neighbors(const bool* board, const uint64_t height, const uint64_t width, const uint64_t y_pos, const uint64_t x_pos) {
  /*
   * increments the neighbors field of the surrounding neighbor
   * cells.
   * */

  uint_fast8_t count = 0;

  int y = -1;
  for (y = -1; y <= 1; y++) {
    int x = -1;
    for (x = -1; x <= 1; x++) {

      if (x == 0 && y == 0) {
        continue;
      }

      if (board[(y + y_pos) * (height + 2) + x + x_pos + 2]) {
        count++;
      }
    }
  }

  return count;

}

__global__
void advance_board(const bool* const current_gen, bool* const next_gen, const uint64_t height, const uint64_t width) {


  // uint64_t x = blockDim.x * blockIdx.x + threadIdx.x;
  // uint64_t y = blockDim.y * blockIdx.y + threadIdx.y;

  uint64_t index = blockIdx.x * blockDim.x + threadIdx.x;
  // uint64_t yindex = blockIdx.y * blockDim.y + threadIdx.y;

  // index will be between 1 and height * width

  uint64_t y = index / (height + 2); 

  uint64_t x = index - (y * (height + 2));

  if (y < 1 || y > height || x < 1 || x > width) {
    // if out of bounds
    return;
  }

  bool current_cell = current_gen[y * (height + 2) + x];

  uint_fast8_t count = check_neighbors(current_gen, height, width, y, x);

  if (current_cell && ( count > 3 || count < 2)) {
    current_cell = false;
  }
  else if (! current_cell && count == 3) {
    current_cell = true;
  }

  next_gen[y * (height + 2) + x + 2] = current_cell;
}

__global__
void white_board(bool* const board, const uint64_t height, const uint64_t width) {

  uint64_t xindex = blockIdx.x * blockDim.x + threadIdx.x;
  uint64_t yindex = blockIdx.y * blockDim.y + threadIdx.y;

  uint64_t xstride = blockDim.x;
  uint64_t ystride = blockDim.y;

  for (size_t y = yindex; y <= height; y += ystride) {
    for (size_t x = xindex; x <= width; x += xstride) {

      board[y * height + x] = true;
    }
  }
}

/*
void copy_board(board_t dest, board_t src) {
  size_t y = 0;
  for (y = 0; y < src.height + 2; y++) {
    memcpy(dest.cells[y], src.cells[y], sizeof(bool) * src.height + 2);
  }
}
*/

void printboard(bool* board, const uint64_t height, const uint64_t width) {
  size_t y = 1;
  for (y = 1; y <= height; y++ ){
    size_t x = 1;
    for (x = 1; x <= width; x++) {
      printf(board[y * height + x] ? "█" : "░");
    }
    puts("");
  }
}

void printfullboard(bool* board, const uint64_t height, const uint64_t width) {
  size_t y = 0;
  for (y = 0; y < height + 2; y++ ){
    size_t x = 0;
    for (x = 0; x < width + 2; x++) {
      printf(board[y * height + x] ? "█" : "░");
    }
    puts("");
  }
}

/*
void free_board(board_t board) {
  free(board.cells);
}
*/

