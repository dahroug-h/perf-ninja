#include "solution.hpp"
#include <algorithm>

bool solution(MatrixOfDoubles &in, MatrixOfDoubles &out) {
  constexpr int I_TILE_SIZE = 32;
  constexpr int J_TILE_SIZE = 64;

  int size = in.size();

  for (int j_block = 0; j_block < size; j_block += J_TILE_SIZE) {
    const int j_max = std::min(j_block + J_TILE_SIZE, size);

    for (int i_block = 0; i_block < size; i_block += I_TILE_SIZE) {
      const int i_max = std::min(i_block + I_TILE_SIZE, size);

      for (int j = j_block; j < j_max; ++j) {
        for (int i = i_block; i < i_max; ++i) {
          out[i][j] = in[j][i];
        }
      }
    }
  }

  return out[0][size - 1];
}
