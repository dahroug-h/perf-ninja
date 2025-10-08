#include "solution.hpp"
#include <algorithm>
#ifdef __cpp_lib_hardware_interference_size
    using std::hardware_constructive_interference_size;
    using std::hardware_destructive_interference_size;
#else
    // 64 bytes on x86-64 │ L1_CACHE_BYTES │ L1_CACHE_SHIFT │ __cacheline_aligned │ ...
    constexpr std::size_t hardware_constructive_interference_size = 64;
    constexpr std::size_t hardware_destructive_interference_size = 64;
#endif

static constexpr int cache_line = hardware_constructive_interference_size;
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
