#include "solution.hpp"
#include <algorithm>

bool solution(MatrixOfDoubles &in, MatrixOfDoubles &out) {
  static constexpr int TILE_SIZE = 16;
  int size = in.size();
  for (int i = 0; i < size; i += TILE_SIZE) {
    for (int j = 0; j < size; j += TILE_SIZE) {
      for (int m = i; m < std::min(i + TILE_SIZE, size); m++) {
        for (int n = j; n < std::min(j + TILE_SIZE, size); n++) {
          out[m][n] = in[n][m];
        }
      }
    }
  }
  return out[0][size - 1];
}
