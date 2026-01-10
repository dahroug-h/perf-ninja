#include "solution.hpp"
#include <algorithm>

bool solution1(MatrixOfDoubles &in, MatrixOfDoubles &out) {
  int size = in.size();
  for (int i = 0; i < size; i++) {
    for (int j = 0; j < size; j++) {
      out[i][j] = in[j][i];
    }
  }
  return out[0][size - 1];
}

bool solution2(MatrixOfDoubles &in, MatrixOfDoubles &out) {
  int size = in.size();
  static constexpr int tile_size = 16;
  for (int ib = 0; ib < size; ib += tile_size) {
    int inx = std::min(ib + tile_size, size);
    for (int jb = 0; jb < size; jb += tile_size) {
      int jnx = std::min(jb + tile_size, size);
      for (int i = ib; i < inx; i++) {
        for (int j = jb; j < jnx; j++) {
          out[i][j] = in[j][i];
        }
      }
    }
  }
  return out[0][size - 1];
}

bool solution3(MatrixOfDoubles &in, MatrixOfDoubles &out) {
  int size = in.size();
  static constexpr int tile_size = 16;
  for (int i = 0; i < size; i += tile_size) {
    for(int j = 0; j < size; ++j) {
      int count = std::min(size, i + tile_size);
      for(int k = i; k < count; ++k) {
        out[k][j] = in[j][k];
      }
    }
  }


  return out[0][size - 1];
}

bool solution(MatrixOfDoubles &in, MatrixOfDoubles &out) {
  return solution3(in, out);
}
