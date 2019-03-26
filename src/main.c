#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <unistd.h>

#include <mpi.h>

#include "main.h"
#include "game.h"

int getopt(int argc, char * const argv[], const char *optstring);
extern char *optarg;
extern int optind, opterr, optopt;

int main(int argc, char* argv[]) {

  board_t cur_gen;
  board_t next_gen;
  game_t game;

  int opt;
  uint32_t seed = (uint32_t) time(NULL);

  while ((opt = getopt(argc, argv, "t:s:h")) != -1) {
    switch(opt) {
      case 's':
        seed = (uint32_t) strtoul(optarg, NULL, 10);
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

  // Set up openmpi
  MPI_Init(&argc, &argv);

  MPI_Comm_size(MPI_COMM_WORLD, &game.threads);
  MPI_Comm_rank(MPI_COMM_WORLD, &game.rank);

  // cut down the board size for each process
  // height /= game.threads;

  /*
  if (game.rank == 0) {
    printf("Width: %lu, Height: %lu, Generations: %lu, Threads: %d\n",
            width * (uint32_t)game.threads,
            height,
            generations,
            game.threads);
  }*/

  height /= game.threads;

  // printf("%d\n", game.rank);
  int seed_start = game.rank;

  // Randomize board state
  cur_gen = randomboard(width, height, seed_start);

  // allocate a board for the next state
  next_gen = blankboard(width, height);

  for (uint64_t gen = 0; gen < generations; gen++) {

#ifdef DEBUG
    // Visualize
    // drawboard(renderer, board);
    printfullboard(cur_gen);
    puts("");
#endif
  
    // Simulate generation
    step_generation(cur_gen, next_gen);
    copyboard(game, cur_gen, next_gen);

  }

  // cleanup
  freecells(cur_gen);
  freecells(next_gen);

  MPI_Finalize();
}

