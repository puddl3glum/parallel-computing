#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <time.h>

#ifdef DEBUG
  #include <SDL2.h>
#endif

#include "main.h"

int main(int argc, char* argv[]) {

#ifdef DEBUG
  SDL_Renderer* renderer;
  SDL_Window* window;
#endif

  // Read in board state
  // OR
  // Randomize board state
  board_t board = randomboard(100, 100);
  
  // Visualize
  
  // Simulate for max number of generations
  
  // cleanup
  freeboard(board);
}

board_t randomboard(const uint64_t n, const uint64_t m) {
  /*
   * Return a 1-dimensional array (N+2)x(M+2) long with random values.
   * The values are initialized from 1..N and 1..M.
   * The edges are treated as ghost cells.
   * */

  // First and last N+2 are false.
  // For every chunk, the first and last are false.
  
  srand((uint64_t)time(NULL));
  
  cell_t** cells = calloc(sizeof(cell_t*), m+2);

  for (size_t i = 0; i < m+2; i++) {
    
    cell_t* const row = calloc(sizeof(cell_t), n+2);
    cells[i] = row;

    for (size_t j = 1; j <= n; j++) {
      bool val = rand() & 1;
      if (val) {
        row[j].alive = true;
        // not necessarily true, but ensure the cell will be examined in the first
        // iteration
        row[j].aliveadj = true;
      }
    }
  }

  const board_t board = {n, m, cells};

  return board; 
}

void freeboard(board_t b) {
  for (size_t i = 0; i < b.m; i++) {
    free(b.cells[i]);
  }

  free(b.cells);
}
