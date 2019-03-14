#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <unistd.h>
#include <omp.h>

#include "main.h"
#include "game.h"

// #ifdef DEBUG
  // #include <SDL2/SDL.h>
  // #include "draw.h"
// #endif


int getopt(int argc, char * const argv[], const char *optstring);
extern char *optarg;
extern int optind, opterr, optopt;

int main(int argc, char* argv[]) {

  int opt;
  uint32_t seed = (uint32_t) time(NULL);

  uint32_t threads = 1;


  while ((opt = getopt(argc, argv, "t:s:h")) != -1) {
    switch(opt) {
      case 's':
        seed = (uint32_t) strtoul(optarg, NULL, 10);
        break;
      case 't':
        threads = strtoul(optarg, NULL, 10);
        break;
      case 'h':
      default:
        fprintf(stderr, "USAGE: %s [OPTION...] width height generations\n", argv[0]);
        fprintf(stderr, "\n");
        fprintf(stderr, "-s[seed]\tSeed for the random number generator.\n");
        fprintf(stderr, "-s[threads]\tSet the number of threads for the program. Default: 1.\n");
        exit(0);
        break;
    }
  
  }


  if (argc - optind < 3) {
    fprintf(stderr, "ERROR: Missing required arguments for width, height, and generations.\n");
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

// #ifdef DEBUG
  // SDL_Event event;
  // SDL_Renderer* renderer;
  // SDL_Window* window;

  // SDL_Init(SDL_INIT_VIDEO);
  // SDL_CreateWindowAndRenderer((int) width, (int) height, 0, &window, &renderer);
// #endif


  // Read in board state
  // OR
  // Randomize board state
  board_t board = randomboard(width, height, seed);

  temp = blankboard(width, height);

  // get new cyclesum tracker
  cyclesum_t cyclesum = newcyclesum(width, height, maxcycles);

  # pragma omp parallel num_threads(threads)
  # pragma omp target teams num_teams(1)
  {

    // omp_set_num_threads(threads);

    for (uint64_t gen = 0; gen < generations; gen++) {

#ifdef DEBUG
      // Visualize
      // drawboard(renderer, board);
      printfullboard(board);
      puts("");
#endif
    
      // Simulate generation
      simgen(board);

      if ( checkcycles(&cyclesum, board) ) break;
      
      // break;
    }

  }
  
// #ifdef DEBUG
  // SDL_DestroyRenderer(renderer);
  // SDL_DestroyWindow(window);
  // SDL_Quit();
// #endif
  
  // cleanup
  freecells(board);
  freecells(temp);
  freecyclesum(cyclesum);
}

