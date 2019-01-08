#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>

#include "game.h"

board_t randomboard(const uint64_t rowlen, const uint64_t colsize) {
  /*
   * Return a 1-dimensional array (N+2)x(M+2) long with random values.
   * The values are initialized from 1..N and 1..M.
   * The edges are treated as ghost cells.
   * */

  // First and last N+2 are false.
  // For every chunk, the first and last are false.
  
  srand((uint64_t)time(NULL));
  
  cell_t** cells = calloc(sizeof(cell_t*), colsize+2);

  for (size_t i = 0; i < colsize+2; i++) {
    
    cell_t* const row = calloc(sizeof(cell_t), rowlen+2);
    cells[i] = row;

    for (size_t j = 1; j <= rowlen; j++) {
      bool val = rand() & 1;
      if (val) {
        row[j].alive = true;
        // not necessarily true, but ensure the cell will be
        // examined in the first iteration
        row[j].aliveadj = true;
      }
    }
  }

  const board_t board = {rowlen, colsize, cells};

  return board; 
}

void printboard(board_t board) {
  for (size_t i = 1; i <= board.col; i++ ){
    for (size_t j = 1; j <= board.row; j++) {
      // printf(" %d", board.cells[i][j].alive);
      printf(board.cells[i][j].alive ? "█" : " ");
    }
    puts("");
  }
}

void freeboard(board_t b) {
  for (size_t i = 0; i < b.col; i++) {
    free(b.cells[i]);
  }

  free(b.cells);
}
