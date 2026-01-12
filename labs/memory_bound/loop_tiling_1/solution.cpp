#include "solution.hpp"
#include <algorithm>
#include <cassert>

constexpr int ROW_TILE_SIZE = 250;
constexpr int COL_TILE_SIZE = 1;
#define SOLUTION

inline int div_func(int a, int b) { return (a ) / b; }

bool solution(MatrixOfDoubles &in, MatrixOfDoubles &out) {
  int size = in.size();
 
#ifndef SOLUTION

  #else
  int row_step = div_func(size,ROW_TILE_SIZE);
  int col_step = div_func(size, COL_TILE_SIZE);
  for (int row_tile_idx = 0; row_tile_idx < size / ROW_TILE_SIZE;
       row_tile_idx++) {
    for (int col_tile_idx = 0; col_tile_idx < size ;
         col_tile_idx++) {
   
        for (int i = 0; i < ROW_TILE_SIZE; i++) {
          out[col_tile_idx ][row_tile_idx * ROW_TILE_SIZE + i] =
              in[row_tile_idx * ROW_TILE_SIZE + i][col_tile_idx];
        }
      
    }
  }

  // std::cout << (col_step * COL_TILE_SIZE) << " " << (row_step * ROW_TILE_SIZE)<<std::endl;
  for (int i = col_step * COL_TILE_SIZE; i < size; i++) {
    for (int j = row_step * ROW_TILE_SIZE; j < size; j++) {
      out[i][j] = in[j][i];
    }
  }

 

  for (int i = row_step * ROW_TILE_SIZE; i < size; i++) {
    for (int j = 0; j < size; j++) {
      out[j][i] = in[i][j];
    }
  }
#endif
  return out[0][size - 1];
}
