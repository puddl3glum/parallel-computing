#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>

#include <mpi.h>
// #include <sys/sysinfo.h>

#include "game.h"

#define ANSI_COLOR_INVERT "\x1b[7m"
#define ANSI_COLOR_RESET   "\x1b[0m"

int_fast8_t checkneighbors(board_t, const uint64_t, const uint64_t);

board_t blankboard(const uint64_t, const uint64_t);
cell_t* blankcells(const uint64_t, const uint64_t);

cell_t* blankcells(const uint64_t width, const uint64_t height) {

  cell_t* cells = calloc(sizeof(cell_t*), (height+2) * (width + 2));

  // for (size_t h = 0; h < height + 2; h++) {
    // cell_t* colptr = calloc(sizeof(cell_t), width+2);
    // cells[h] = colptr;
  // }

  return cells;
}

board_t blankboard(const uint64_t width, const uint64_t height) {
  /*
   * Return a 1-dimensional array (N+2)x(M+2) long with random values.
   * The values are initialized from 1..N and 1..M.
   * The edges are treated as ghost cells.
   * */

  // First and last N+2 are false.
  // For every chunk, the first and last are false.
  
  board_t board = {width, height, blankcells(width, height)};

  return board; 

}

board_t randomboard(const uint64_t width, const uint64_t height, const uint32_t seed) {
  /*
   * Return a 1-dimensional array (N+2)x(M+2) long with random values.
   * The values are initialized from 1..N and 1..M.
   * The edges are treated as ghost cells.
   * */

  // First and last N+2 are false.
  // For every chunk, the first and last are false.
  // printf("%d\n", seed);
  
  srand(seed);

  board_t board = blankboard(width, height);

  for (size_t h = 1; h <= board.height; h++) {
    for (size_t w = 1; w <= board.width; w++) {
      int n = rand();
      bool val = n % 2 == 0;
      if (val == 1) {
        board.cells[h * board.height + w].alive = true;
      }
    }
  }

  return board; 
}

int_fast8_t checkneighbors(board_t board, const uint64_t x, const uint64_t y) {
  // inc and dec neighbors were not working w/ the new way of doing openmpi
  // had to change to old-fashioned neighbor checking
  /*
   * increments the neighbors field of the surrounding neighbor
   * cells.
   * */

  int_fast8_t count = 0;

  for (int i = -1; i <= 1; i++) {
    for (int j = -1; j <= 1; j++) {

      if (i == 0 && j == 0) continue;

      // printf("\tCell neighbor: (%ld, %ld)\n", col + y, row + x);
      if (board.cells[(y + i) * board.height + (x + j)].alive) {
        count++;
      }
    }
  }

  return count;
}

void step_generation(board_t cur_gen, board_t next_gen) {

  for (size_t h = 1; h <= cur_gen.height; h++) {
    for (size_t w = 1; w <= cur_gen.width; w++) {

      // printf("%d\n", omp_get_num_threads());

      // copy old cell to new cell
      // no pointer needed to val which won't be changed
      // cell_t tempcell = temp.cells[row][col];
      const cell_t curcell = cur_gen.cells[h * cur_gen.height + w];

      int_fast8_t neighbors = checkneighbors(cur_gen, w, h);

      // fewer than 2 neighbors, cell dies
      if (curcell.alive && (neighbors < 2 || neighbors > 3)) {

        next_gen.cells[h * next_gen.height + w].alive = false;
        // update neighbors of surrounding cells
      }
      else if (! curcell.alive && neighbors == 3) {
        next_gen.cells[h * next_gen.height + w].alive = true;
      }
    }
  }
}

void copyboard(game_t game, board_t dest, board_t src) {

  
  // copy the top and bottom
  if (game.threads == 1) {
    for (size_t w = 1; w <= src.width; w++) {
      src.cells[0 * src.height + w] = src.cells[src.height * src.height + w];
      src.cells[(dest.height + 1) * dest.height + w] = src.cells[1 * src.height + w];
    }
  }
  else {
    int left = (game.rank - 1 + game.threads) % game.threads;
    int right = (game.rank + 1) % game.threads;

    MPI_Status status;

    MPI_Sendrecv(src.cells,
        (int) src.width + 2,
        MPI_UINT8_T,
        right,
        1,
        src.cells,
        (int) src.width + 2,
        MPI_UINT8_T,
        left,
        1,
        MPI_COMM_WORLD,
        &status);

    MPI_Sendrecv(src.cells,
        (int) src.width + 2,
        MPI_UINT8_T,
        left,
        0,
        src.cells,
        (int) src.width + 2,
        MPI_UINT8_T,
        right,
        0,
        MPI_COMM_WORLD,
        &status);
  }
  
  // copy the left and right of the board
  for (size_t h = 1; h <= src.height; h++) {
    src.cells[h * src.height + 0] = src.cells[h * src.height + src.width];
    src.cells[h * src.height + (src.width + 1)] = src.cells[h * src.height + 1];
  }

  // copy corners
  dest.cells[0 * src.height + 0] = src.cells[src.height * src.height + src.width];
  dest.cells[0 * src.height + dest.width + 1] = src.cells[src.height * src.height + 1];
  dest.cells[(dest.height + 1 * src.height) + dest.width + 1] = src.cells[1 * src.height + 1];
  dest.cells[(dest.height + 1) * src.height + 0] = src.cells[1 * src.height + src.width];
 

  for (size_t h = 1; h <= src.height; h++) {
    for (size_t w = 1; w <= src.width; w++) {
      dest.cells[h * dest.height  +w] = src.cells[h * src.height + w];
    }
  }
}

void printboard(board_t board) {
  for (size_t h = 1; h <= board.height; h++) {
    for (size_t w = 1; w <= board.width; w++ ){
      printf(board.cells[h * board.height + w].alive ? "█" : "░");
    }
    puts("");
  }
  // usleep(200 * 1000);
}

void printfullboard(board_t board) {
  for (size_t h = 0; h < board.height + 2; h++) {
    for (size_t w = 0; w < board.width + 2; w++ ){

      // if (board.cells[h][w].alive) printf(ANSI_COLOR_INVERT);
      // printf("%d", board.cells[h][w].neighbors);
      // printf(ANSI_COLOR_RESET);
      printf(board.cells[h * board.height + w].alive ? "█" : "░");
      
    }
    puts("");
  }
}

void freecells(board_t b) {
  // for (size_t h = 0; h < b.height + 2; h++) {
    // free(b.cells[h]);
  // }

  free(b.cells);
}

