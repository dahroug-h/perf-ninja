#include "solution.hpp"
#include <algorithm>

bool solution(MatrixOfDoubles &in, MatrixOfDoubles &out) {
  constexpr int TILE_SIZE = 64;

  int size = in.size();
  for (int i_block = 0; i_block < size; i_block += TILE_SIZE) {
    const int i_offset_max = std::min(TILE_SIZE, size - i_block);

    for (int j_block = 0; j_block < size; j_block += TILE_SIZE) {
      const int j_offset_max = std::min(TILE_SIZE, size - j_block);

      for (int i_offset = 0; i_offset < i_offset_max; ++i_offset) {
        for (int j_offset = 0; j_offset < j_offset_max; ++j_offset) {
          out[i_block + i_offset][j_block + j_offset] =
              in[j_block + j_offset][i_block + i_offset];
        }
      }
    }
  }

  return out[0][size - 1];
}
