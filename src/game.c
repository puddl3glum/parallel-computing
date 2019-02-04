#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>
#include <omp.h>
// #include <sys/sysinfo.h>

#include "game.h"

void incneighbors(board_t, const uint64_t, const uint64_t);
void decneighbors(board_t, const uint64_t, const uint64_t);
void copyboard(board_t, board_t);
board_t blankboard(const uint64_t, const uint64_t);
cell_t** blankcells(const uint64_t, const uint64_t);


cell_t** blankcells(const uint64_t row, const uint64_t col) {

  cell_t** cells = calloc(sizeof(cell_t*), col+2);

  for (size_t r = 0; r < col+2; r++) {
    cell_t* rowptr = calloc(sizeof(cell_t), row+2);
    cells[r] = rowptr;
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

board_t randomboard(const uint64_t rowlen, const uint64_t colsize) {
  /*
   * Return a 1-dimensional array (N+2)x(M+2) long with random values.
   * The values are initialized from 1..N and 1..M.
   * The edges are treated as ghost cells.
   * */

  // First and last N+2 are false.
  // For every chunk, the first and last are false.
  
  srand((uint32_t)time(NULL));

  board_t board = blankboard(rowlen, colsize);

  for (size_t row = 1; row <= board.row; row++) {
    for (size_t col = 1; col <= board.col; col++) {
      bool val = rand() & 1;
      if (val) {
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
      board.cells[(int64_t) row + y][(int64_t) col + x].neighbors++;
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

      board.cells[(int64_t) row + y][(int64_t) col + x].neighbors--;
    }
  }
}

void simgen(board_t board) {

  // create temp board
  board_t temp = blankboard(board.row, board.col);

  // copy old board to new board
  copyboard(temp, board);

  // omp_set_num_threads(get_nprocs());
  omp_set_num_threads(8);

  # pragma omp parallel for
  for (size_t row = 1; row <= board.row; row++) {
    for (size_t col = 1; col <= board.col; col++) {
      
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

  freecells(temp);
}

void copyboard(board_t dest, board_t src) {
  for (size_t row = 0; row < src.col + 2; row++) {
    memcpy(dest.cells[row], src.cells[row], sizeof(cell_t) * src.row + 2);
  }
}

void printboard(board_t board) {
  for (size_t i = 1; i <= board.col; i++ ){
    for (size_t j = 1; j <= board.row; j++) {
      printf(board.cells[i][j].alive ? "█" : "░");
    }
    puts("");
  }
}

void printfullboard(board_t board) {
  for (size_t i = 0; i < board.col + 2; i++ ){
    for (size_t j = 0; j < board.row + 2; j++) {
      printf(board.cells[i][j].alive ? "█" : "░");
    }
    puts("");
  }
}

void freecells(board_t b) {
  for (size_t i = 0; i < b.col + 2; i++) {
    free(b.cells[i]);
  }

  free(b.cells);
}

