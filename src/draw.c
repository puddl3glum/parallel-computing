#include <stdlib.h>
#include <stdio.h>
#include <SDL2/SDL.h>

#include "game.h"
#include "draw.h"

void drawboard(SDL_Renderer* renderer, board_t board) {

  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
  SDL_RenderClear(renderer);

  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

  printf("Col: %ld", board.col);
  printf("Row: %ld", board.row);

  // iterate through board and draw
  for (size_t i = 1; i <= board.col; i++) {
    for (size_t j = 1; i <= board.row; j++) {
      if (board.cells[i][j].alive) {

        SDL_RenderDrawPoint(renderer, i, j);
      }
    }
    puts("");
  }

  SDL_RenderPresent(renderer);
}
