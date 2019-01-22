#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "main.h"
#include "game.h"

#ifdef DEBUG
  #include <SDL2/SDL.h>
  #include "draw.h"
#endif

int main(int argc, char* argv[]) {

  if (argc < 4) {
    printf("USAGE: %s WIDTH HEIGHT GENERATIONS [CYCLE_HISTORY]\n", argv[0]);
    puts("\tWIDTH: width of the board");
    puts("\tHEIGHT: height of the board");
    puts("\tGENERATIONS: The maximum number of generations to simulate");
    puts("\tCYCLE_HISTORY: how far back to check for cycles. default = 2");
    return 1;
  }

  // int boardwidth = 1000;
  // int boardheight = 1000;

  // int maxgenerations = 1000;
  
  const uint64_t boardwidth = strtoul(argv[1], NULL, 10);
  const uint64_t boardheight = strtoul(argv[2], NULL, 10);
  const uint64_t maxgenerations = strtoul(argv[3], NULL, 10);
  const uint64_t maxcycles = (argc == 5) ? strtoul(argv[4], NULL, 10) : 2;


#ifdef DEBUG
  // SDL_Event event;
  SDL_Renderer* renderer;
  SDL_Window* window;

  SDL_Init(SDL_INIT_VIDEO);
  SDL_CreateWindowAndRenderer((int) boardwidth, (int) boardheight, 0, &window, &renderer);
#endif

  // Read in board state
  // OR
  // Randomize board state
  board_t board = randomboard(boardwidth, boardheight);

  // get new cyclesum tracker
  cyclesum_t cyclesum = newcyclesum(boardwidth, boardheight, maxcycles);

  for (uint64_t gen = 0; gen < maxgenerations; gen++) {

#ifdef DEBUG
    // Visualize
    drawboard(renderer, board);
    // printboard(board);
    // puts("");
#endif
  
    // Simulate generation
    simgen(board);

    if ( checkcycles(&cyclesum, board) ) break;


    // compute the checksum of the board
    // sum alive cells in each row
    // add to array
    // default, 1 generation
    // if current array == any in history : quit
    
    // break;
  }
  
#ifdef DEBUG
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
#endif
  
  // cleanup
  freecells(board);
  freecyclesum(cyclesum);
}

