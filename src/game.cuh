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

/*
typedef struct {
  uint64_t height;
  uint64_t width;
  bool* cells;
} board_t;
*/

/*
typedef struct {
  uint64_t maxcycle;
  uint64_t col;
  uint64_t row;
  uint64_t current; // which cycle is currently on
  uint64_t  ** const sums; // array of arrays of row sums
} cyclesum_t;
*/

bool* random_board(uint64_t, uint64_t, const double, const uint64_t);
bool* new_board(const uint64_t, const uint64_t);
// cyclesum_t newcyclesum(uint64_t, uint64_t, uint64_t);
// void freecyclesum(cyclesum_t);
// bool checkcycles(cyclesum_t*, board_t);
void printboard(bool*, const size_t, const size_t);
void printfullboard(bool*, const size_t, const size_t);
__global__
void advance_board(const bool* const, bool* const, const uint64_t, const uint64_t);
