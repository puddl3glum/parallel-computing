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
    printf("USAGE: %s width height generation\n", argv[0]);
    return 1;
  }

  // int boardwidth = 1000;
  // int boardheight = 1000;

  // int maxgenerations = 1000;
  
  uint64_t boardwidth = strtoul(argv[1], NULL, 10);
  uint64_t boardheight = strtoul(argv[2], NULL, 10);
  uint64_t maxgenerations = strtoul(argv[3], NULL, 10);


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

  for (uint64_t gen = 0; gen < maxgenerations; gen++) {

#ifdef DEBUG
    // Visualize
    drawboard(renderer, board);
    // printboard(board);
    // puts("");
#endif
  
    // Simulate generation
    simgen(board);
    
    // break;
  }
  
#ifdef DEBUG
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
#endif
  
  // cleanup
  freecells(board);
}

