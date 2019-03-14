#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <omp.h>
// #include <sys/sysinfo.h>

#include "game.h"

#define ANSI_COLOR_INVERT "\x1b[7m"
#define ANSI_COLOR_RESET   "\x1b[0m"

void incneighbors(board_t, const uint64_t, const uint64_t);
void decneighbors(board_t, const uint64_t, const uint64_t);
void copyboard(board_t, board_t);
board_t blankboard(const uint64_t, const uint64_t);
cell_t** blankcells(const uint64_t, const uint64_t);
bool arraycomp(uint64_t*, uint64_t*, uint64_t);

extern board_t temp;

cyclesum_t newcyclesum(const uint64_t row, const uint64_t col, uint64_t maxcycles) {
  /*
   * Return a new cyclesum_t struct
   * */

  maxcycles = maxcycles + 1; // add 1 to make space for the cycle being summed

  // create an array for holding sums of length maxcycle
  uint64_t** const sums = calloc(sizeof(uint64_t*), maxcycles);

  for (size_t x = 0; x < maxcycles; x++) {

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
  for (size_t x = 0; x < cyclesum.maxcycle; x++) {
    free(cyclesum.sums[x]);
  }

  free(cyclesum.sums);
}

bool arraycomp(uint64_t* a, uint64_t* b, uint64_t len) {

  for (size_t x = 0; x < len; x++) {
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

  for (size_t x = 1; x <= board.col; x++ ) {
    uint64_t sum = 0;

    for (size_t y = 1; y <= board.row; y++) {
      if (board.cells[x][y].alive) {
        sum++;
      }
    }

    // set sum to cyclesum current
    cyclesum->sums[cyclesum->current][x - 1] = sum;
  }

  // compare current to other cycles
  for (size_t x = 0; x < cyclesum->maxcycle; x++) {
    uint64_t currentcycle = cyclesum->current;

    if (x != currentcycle) {
      if ( arraycomp(cyclesum->sums[x], cyclesum->sums[currentcycle], cyclesum->row)) return true;
    }
  }

  // increment cycle
  // NOTE: there is no need to increment if two cycles matched
  cyclesum->current = (cyclesum->current + 1) % cyclesum->maxcycle;

  return false;
}

cell_t** blankcells(const uint64_t row, const uint64_t col) {

  cell_t** cells = calloc(sizeof(cell_t*), row+2);

  for (size_t r = 0; r < row+2; r++) {
    cell_t* colptr = calloc(sizeof(cell_t), col+2);
    cells[r] = colptr;
  }

  return cells;
}

board_t blankboard(const uint64_t row, const uint64_t col) {
  /*
   * Return a 1-dimensional array (N+2)x(M+2) long with random values.
   * The values are initialized from 1..N and 1..M.
   * The edges are treated as ghost cells.
   * */

  // First and last N+2 are false.
  // For every chunk, the first and last are false.
  
  board_t board = {row, col, blankcells(row, col)};

  return board; 

}

board_t randomboard(const uint64_t rowlen, const uint64_t colsize, const uint32_t seed) {
  /*
   * Return a 1-dimensional array (N+2)x(M+2) long with random values.
   * The values are initialized from 1..N and 1..M.
   * The edges are treated as ghost cells.
   * */

  // First and last N+2 are false.
  // For every chunk, the first and last are false.
  // printf("%d\n", seed);
  
  srand(seed);

  board_t board = blankboard(rowlen, colsize);

  for (size_t row = 1; row <= board.row; row++) {
    for (size_t col = 1; col <= board.col; col++) {
      int n = rand();
      bool val = n % 2 == 0;
      // printf("%d", val);
      if (val == 1) {
        board.cells[row][col].alive = true;
        // not necessarily true, but ensure the cell will be
        // examined in the first iteration
        incneighbors(board, row, col);
      }
    }
  }

  return board; 
}

void incneighbors(board_t board, const uint64_t row, const uint64_t col) {
  /*
   * increments the neighbors field of the surrounding neighbor
   * cells.
   * */

  for (int x = -1; x <= 1; x++) {
    for (int y = -1; y <= 1; y++) {

      if (x == 0 && y == 0) continue;

      // printf("\tCell neighbor: (%ld, %ld)\n", col + y, row + x);
      # pragma omp atomic
      board.cells[(int64_t) row + x][(int64_t) col + y].neighbors++;
    }
  }
}

void decneighbors(board_t board, const uint64_t row, const uint64_t col) {
  /*
   * decrements the neighbors field of the surrounding neighbor
   * cells.
   * */

  for (int x = -1; x <= 1; x++) {
    for (int y = -1; y <= 1; y++) {

      if (x == 0 && y == 0) continue;

      # pragma omp atomic
      board.cells[(int64_t) row + x][(int64_t) col + y].neighbors--;
    }
  }
}

void simgen(board_t board) {


  // create temp board
  // board_t temp = blankboard(board.row, board.col);

  // copy old board to new board
  copyboard(temp, board);

  // this program is cpu-heavy. 1 proc per thread
  // omp_set_num_threads(get_nprocs());

  # pragma omp distribute parallel for collapse(2)
  for (size_t row = 1; row <= board.row; row++) {
    for (size_t col = 1; col <= board.col; col++) {

      // printf("%d\n", omp_get_num_threads());

      // copy old cell to new cell
      // no pointer needed to val which won't be changed
      cell_t tempcell = temp.cells[row][col];

      // fewer than 2 neighbors, cell dies
      if (tempcell.alive && (tempcell.neighbors < 2 || tempcell.neighbors > 3)) {

        board.cells[row][col].alive = false;
        // update neighbors of surrounding cells
        decneighbors(board, row, col);
      }
      else if (! tempcell.alive && tempcell.neighbors == 3) {
        board.cells[row][col].alive = true;
        incneighbors(board, row, col);
      }
    }
  }

  // freecells(temp);
}

void copyboard(board_t dest, board_t src) {
  /*
  for (size_t row = 0; row < src.row + 2; row++) {
     memcpy(dest.cells[row], src.cells[row], sizeof(cell_t) * src.col + 2);
  }
  */
  
  
  for (size_t row = 1; row <= src.row; row++) {
    for (size_t col = 1; col <= src.col; col++) {
      dest.cells[row][col] = src.cells[row][col];
    }
  }
  
}

void printboard(board_t board) {
  for (size_t i = 1; i <= board.row; i++ ){
    for (size_t j = 1; j <= board.col; j++) {
      // printf(board.cells[i][j].alive ? "█" : "░");
    }
    puts("");
  }
}

void printfullboard(board_t board) {
  for (size_t i = 0; i < board.row + 2; i++ ){
    for (size_t j = 0; j < board.col + 2; j++) {
      // printf(board.cells[i][j].alive ? "█" : "░");
      if (board.cells[i][j].alive) printf(ANSI_COLOR_INVERT);
      printf("%d", board.cells[i][j].neighbors);
      printf(ANSI_COLOR_RESET);
    }
    puts("");
  }
}

void freecells(board_t b) {
  for (size_t i = 0; i < b.row + 2; i++) {
    free(b.cells[i]);
  }

  free(b.cells);
}

