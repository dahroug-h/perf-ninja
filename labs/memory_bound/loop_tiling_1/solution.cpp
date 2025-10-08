#include "solution.hpp"
#include <algorithm>

bool solution(MatrixOfDoubles &in, MatrixOfDoubles &out) {
  int size = in.size();
  for (int i = 0; i < size; i += 8) {
    for (int j = 0; j < size; j += 8) {
      for (int m = i; m < std::min(i + 8, size); m++) {
        for (int n = j; n < std::min(j + 8, size); n++) {
          out[m][n] = in[n][m];
        }
      }
      // out[i][j] = in[j][i];
    }
  }
  return out[0][size - 1];
}
