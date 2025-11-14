#include "solution.hpp"

static std::size_t mapToBucket(std::size_t v) {
  static std::size_t lut[100] = {
       0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
       1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
       2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,
       3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,
       4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,
       5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,
       6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6
  };
  return v < 100 ? lut[v] : DEFAULT_BUCKET;
}

std::array<std::size_t, NUM_BUCKETS> histogram(const std::vector<int> &values) {
  std::array<std::size_t, NUM_BUCKETS> retBuckets{0};
  for (auto v : values) {
    retBuckets[mapToBucket(v)]++;
  }
  return retBuckets;
}
