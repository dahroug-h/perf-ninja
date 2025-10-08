#include "solution.hpp"
#include <algorithm>

static constexpr int cache_line = std::hardware_constructive_interference_size;
bool solution(MatrixOfDoubles &in, MatrixOfDoubles &out) {
  int size = in.size();
  int count_per_cache_line = cache_line / sizeof(double);
  for (int i = 0; i < size; i += count_per_cache_line) {
    for (int j = 0; j < size; j += count_per_cache_line) {
      for (int m = i; m < std::min(i + count_per_cache_line, size); m++) {
        for (int n = j; n < std::min(j + count_per_cache_line, size); n++) {
          out[m][n] = in[n][m];
        }
      }
    }
  }
  return out[0][size - 1];
}
