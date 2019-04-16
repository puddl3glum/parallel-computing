/*
 * William Setzer
 * wjsetzer
 * CS 732
 * Homework 1
 * */

#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

typedef struct {
  uint64_t height;
  uint64_t width;
  bool** cells;
} board_t;

typedef struct {
  uint64_t maxcycle;
  uint64_t col;
  uint64_t row;
  uint64_t current; // which cycle is currently on
  uint64_t  ** const sums; // array of arrays of row sums
} cyclesum_t;

void free_board(board_t);
board_t random_board(uint64_t, uint64_t, double, size_t);
board_t new_board(uint64_t, uint64_t);
cyclesum_t newcyclesum(uint64_t, uint64_t, uint64_t);
void freecyclesum(cyclesum_t);
bool checkcycles(cyclesum_t*, board_t);
void printfullboard(board_t);
void printboard(board_t);
void advance_board(board_t, board_t);
