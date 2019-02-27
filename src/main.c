#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <unistd.h>

#include "main.h"
#include "game.h"

#ifdef DEBUG
  #include <SDL2/SDL.h>
  #include "draw.h"
#endif

int main(int argc, char* argv[]) {

  int opt;
  uint32_t seed = (uint32_t) time(NULL);


  while ((opt = getopt(argc, argv, "s:")) != -1) {
    switch(opt) {
      case 's':
        seed = (uint32_t) strtoul(optarg, NULL, 10);
        break;
      default:
        fprintf(stderr, "USAGE: %s [OPTION...] width height generations\n", argv[0]);
        exit(0);
        break;
    }
  
  }


  if (optind >= argc) {
    fprintf(stderr, "Expected arguments after options\n");
    exit(EXIT_FAILURE);
  }


  uint64_t width = strtoul(argv[0 + optind], NULL, 10);
  uint64_t height = strtoul(argv[1 + optind], NULL, 10);
  uint64_t generations = strtoul(argv[2 + optind], NULL, 10);

  const uint64_t maxcycles = 12;

  // printf("%ld %ld %ld\n", width, height, generations);

  // printf("%ld\n", width);

  // if (argc < 4) {
  //  return 1;
  // }


  // int boardwidth = 1000;
  // int boardheight = 1000;

  // int maxgenerations = 1000;
  
  // uint32_t seed = (uint32_t) strtoul(argv[4], NULL, 10);

#ifdef DEBUG
  // SDL_Event event;
  SDL_Renderer* renderer;
  SDL_Window* window;

  SDL_Init(SDL_INIT_VIDEO);
  SDL_CreateWindowAndRenderer((int) width, (int) height, 0, &window, &renderer);
#endif


  // Read in board state
  // OR
  // Randomize board state
  board_t board = randomboard(width, height, seed);

  // get new cyclesum tracker
  cyclesum_t cyclesum = newcyclesum(width, height, maxcycles);

  for (uint64_t gen = 0; gen < generations; gen++) {

#ifdef DEBUG
    // Visualize
    drawboard(renderer, board);
    // printfullboard(board);
    // puts("");
#endif
  
    // Simulate generation
    simgen(board);

    if ( checkcycles(&cyclesum, board) ) break;
    
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

