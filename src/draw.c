#include <stdlib.h>
#include <stdio.h>
#include <SDL2/SDL.h>

#include "game.h"
#include "draw.h"

void drawboard(SDL_Renderer* renderer, board_t board) {

  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
  SDL_RenderClear(renderer);

  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

  // printf("Col: %ld\n", board.col);
  // printf("Row: %ld\n", board.row);

  // iterate through board and draw
  for (size_t col = 1; col <= board.col; col++) {
    // printf("%ld ", i);
    for (size_t row = 1; row <= board.row; row++) {
      // printf("%ld ", j);
      if (board.cells[row][col].alive) {

        SDL_RenderDrawPoint(renderer, row, col);
      }
    }
  }

  SDL_RenderPresent(renderer);
  SDL_Delay(150);
}
