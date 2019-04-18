
/*
 * William Setzer
 * wjsetzer
 * CS 732
 * Homework 1
 * */

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>

#include "main.cuh"
#include "game.cuh"

// int getopt(int argc, char * const argv[], const char *optstring);
// extern char* optarg;
// extern int optind, opterr, optopt;
#define gpuErrchk(ans) { gpuAssert((ans), __FILE__, __LINE__); }
inline void gpuAssert(cudaError_t code, const char *file, int line, bool abort=true)
{
  if (code != cudaSuccess){
    fprintf(stderr,"GPUassert: %s %s %d\n", cudaGetErrorString(code), file, line);

    if (abort) {
      exit(code);
    }
  }
}

int main(int argc, char* argv[]) {


  int opt;

  uint64_t width = 100;
  uint64_t height = 100;
  uint64_t generations = 100;
  double chance = 0.5;
  uint64_t seed = 1;


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
    }
  }

  // Read in board state
  // OR
  // Randomize board state
  bool* current_gen = random_board(height, width, chance, seed);
  bool* next_gen = new_board(height, width);

  // memcpy(next_gen, current_gen, (height + 2) * (width + 2) * sizeof(bool));
  // get new cyclesum tracker
  // cyclesum_t cyclesum = newcyclesum(width, height, maxcycles);
  
  // create CUDA space
  bool* cuda_current_gen;
  bool* cuda_next_gen;
  gpuErrchk(cudaMalloc(&cuda_current_gen, (height + 2) * (width + 2) * sizeof(bool)));
  gpuErrchk(cudaMalloc(&cuda_next_gen, (height + 2) * (width + 2) * sizeof(bool)));

  // printf("%p\n", cuda_current_gen);
  // printf("%p\n", cuda_next_gen);

  // memcpy(cuda_current_gen, current_gen, (height + 2) * (width + 2) * sizeof(bool));
  // puts("here");

  cudaMemcpy(cuda_current_gen, current_gen, (height + 2) * (width + 2) * sizeof(bool), cudaMemcpyHostToDevice);
  // cudaMemcpy(cuda_next_gen, current_gen, (height + 2) * (width + 2) * sizeof(bool), cudaMemcpyHostToDevice);

  current_gen = new_board(height, width);

  // cudaMemcpy(current_gen, cuda_current_gen, (height + 2) * (width + 2) * sizeof(bool), cudaMemcpyDeviceToHost);
  printboard(cuda_current_gen, height, width);

  uint64_t blocksize = 512;
  uint64_t numblocks =  ((height) * (width) + blocksize - 1) / blocksize;

  size_t gen = 0;
  for (gen = 0; gen < generations; gen++) {

    // Simulate generation
    advance_board<<<numblocks, blocksize>>>(cuda_current_gen, cuda_next_gen, height, width);

    cudaDeviceSynchronize();
    bool* temp;

    temp = cuda_current_gen;
    cuda_current_gen = cuda_next_gen;
    cuda_next_gen = temp;

    // if ( checkcycles(&cyclesum, board) ) break;

    // compute the checksum of the board
    // sum alive cells in each row
    // add to array
    // default, 1 generation
    // if current array == any in history : quit
    
    // break;
  }

  white_board<<<numblocks, blocksize>>>(cuda_next_gen, height, width);

  // cudaMemcpy(current_gen, cuda_current_gen, (height + 2) * (width + 2), cudaMemcpyDeviceToHost);
  cudaMemcpy(next_gen, cuda_next_gen, (height + 2) * (width + 2) * sizeof(bool), cudaMemcpyDeviceToHost);

#ifdef DEBUG
  printboard(current_gen, height, width);
  puts("");
  printboard(next_gen, height, width);
#endif
  
  // cleanup
  cudaFree(cuda_current_gen);
  cudaFree(cuda_next_gen);
  free(current_gen);
  free(next_gen);
  // freecyclesum(cyclesum);
}

