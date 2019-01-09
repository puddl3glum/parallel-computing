#include <stdio.h>
#include <stdlib.h>

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
  
  int boardwidth = atoi(argv[1]);
  int boardheight = atoi(argv[2]);
  int maxgenerations = atoi(argv[3]);


#ifdef DEBUG
  // SDL_Event event;
  SDL_Renderer* renderer;
  SDL_Window* window;

  SDL_Init(SDL_INIT_VIDEO);
  SDL_CreateWindowAndRenderer(boardwidth, boardheight, 0, &window, &renderer);
#endif

  // Read in board state
  // OR
  // Randomize board state
  board_t board = randomboard(boardwidth, boardheight);

  for (int gen = 0; gen < maxgenerations; gen++) {

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

