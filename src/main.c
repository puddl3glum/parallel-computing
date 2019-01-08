#include <stdio.h>

#include "main.h"
#include "game.h"

#ifdef DEBUG
  #include <SDL2/SDL.h>
  #include "draw.h"
#endif

int main(int argc, char* argv[]) {

  int boardwidth = 10;
  int boardheight = 10;

#ifdef DEBUG
  SDL_Event event;
  SDL_Renderer* renderer;
  SDL_Window* window;

  SDL_Init(SDL_INIT_VIDEO);
  SDL_CreateWindowAndRenderer(boardwidth, boardheight, 0, &window, &renderer);
#endif

  // Read in board state
  // OR
  // Randomize board state
  board_t board = randomboard(boardwidth, boardheight);

#ifdef DEBUG
  puts("Board created");
  printboard(board);
#endif
  
  // Visualize
#ifdef DEBUG
  drawboard(renderer, &board);
  puts("Board drawn");
#endif
  
  // Simulate for max number of generations
  
#ifdef DEBUG
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
#endif
  
  // cleanup
  freeboard(board);
}

