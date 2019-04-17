
/*
 * William Setzer
 * wjsetzer
 * CS 732
 * Homework 1
 * */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>

#include "main.h"
#include "game.h"

int getopt(int argc, char * const argv[], const char *optstring);
extern char* optarg;
extern int optind, opterr, optopt;

int main(int argc, char* argv[]) {

  int opt;

  uint64_t width = 100;
  uint64_t height = 100;
  uint64_t generations = 100;
  double chance = 0.5;
  size_t seed = 1;


  while ((opt = getopt(argc, argv, "s:w:h:g:c:")) != -1) {

    switch(opt) {
      case 's':
        // set the seed
        seed = strtoul(optarg, NULL, 10);
        break;
      case 'w':
        width = strtoul(optarg, NULL, 10);
        break;
      case 'h':
        height = strtoul(optarg, NULL, 10);
        break;
      case 'g':
        generations = strtoul(optarg, NULL, 10);
        break;
      case 'c':
        chance = strtod(optarg, NULL);
        break;
      default:
        printf("USAGE: %s [OPTION]\n", argv[0]);
        puts("Options:");
        puts("\ts : The seed to use for the random number generator.");
        puts("\tw : The width for the board.");
        puts("\th : The height for the board.");
        puts("\tg : The number of generations for the game.");
        puts("\tc : The chance a cell will be alive.");
        return 0;
        break;
    }

  }

  // Read in board state
  // OR
  // Randomize board state
  board_t current_gen = random_board(width, height, chance, seed);
  board_t next_gen = new_board(width, height);
  // get new cyclesum tracker
  // cyclesum_t cyclesum = newcyclesum(width, height, maxcycles);

  for (size_t gen = 0; gen < generations; gen++) {

#ifdef DEBUG
    // Visualize
    printboard(current_gen);
    puts("");
#endif
  
    // Simulate generation
    advance_board(current_gen, next_gen);

    board_t temp;

    temp = current_gen;
    current_gen = next_gen;
    next_gen = temp;

    // if ( checkcycles(&cyclesum, board) ) break;

    // compute the checksum of the board
    // sum alive cells in each row
    // add to array
    // default, 1 generation
    // if current array == any in history : quit
    
    // break;
  }
  
  
  // cleanup
  free_board(current_gen);
  free_board(next_gen);
  // freecyclesum(cyclesum);
}

