#include "solution.hpp"

#include <cstdint>

// static std::size_t mapToBucket(std::size_t v) {
//   static std::size_t lut[101] = {
//     // 0   1   2   3   4   5   6   7   8   9  10  11  12  13  14  15  16  17
//        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
//        1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
//        2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,
//        3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,
//        4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,
//        5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,
//        6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,
//        DEFAULT_BUCKET
//   };
//   const size_t idx = __builtin_unpredictable(v < 101) ? v : 100;
//   return lut[idx];
// }

// std::array<std::size_t, NUM_BUCKETS> histogram(const std::vector<int> &values) {
//   std::array<std::size_t, NUM_BUCKETS> retBuckets{0};
//   for (auto v : values) {
//     retBuckets[mapToBucket(v)]++;
//   }
//   return retBuckets;
// }

std::array<std::size_t, NUM_BUCKETS> histogram(const std::vector<int> &values) {
  std::array<std::size_t, NUM_BUCKETS> retBuckets{0};

  static std::size_t lut[101] = {
    // 0   1   2   3   4   5   6   7   8   9  10  11  12  13  14  15  16  17
       0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
       1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
       2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,
       3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,
       4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,
       5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,
       6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,
       DEFAULT_BUCKET
  };  

  for (auto v : values) {
    const size_t idx = __builtin_unpredictable(v < 101) ? v : 100;
    retBuckets[lut[idx]]++;
  }
  return retBuckets;
}
