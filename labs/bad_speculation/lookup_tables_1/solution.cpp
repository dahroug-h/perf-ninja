#include "solution.hpp"
#define SOLUTION

#ifdef SOLUTION
#include <stdint.h>

struct lookup_table {
  uint8_t v[151];
  lookup_table() {
    for (int i = 0; i <= 150; i++) {
      if (i < 13) {
        v[i] = 0;
      } else if (i < 29) {
        v[i] = 1;
      } else if (i < 41) {
        v[i] = 2;
      } else if (i < 53) {
        v[i] = 3;
      } else if (i < 71) {
        v[i] = 4;
      } else if (i < 83) {
        v[i] = 5;
      } else if (i < 100) {
        v[i] = 6;
      } else {
        v[i] = DEFAULT_BUCKET;
      }
    }
  }
};

static lookup_table table;
#endif

static std::size_t mapToBucket(std::size_t v) {
  //   size of a bucket

#ifdef SOLUTION
  return table.v[v];
#else
  if (v < 13)
    return 0;  //   13
  else if (v < 29)
    return 1;  //   16
  else if (v < 41)
    return 2;  //   12
  else if (v < 53)
    return 3;  //   12
  else if (v < 71)
    return 4;  //   18
  else if (v < 83)
    return 5;  //   12
  else if (v < 100)
    return 6;  //   17
  return DEFAULT_BUCKET;
#endif
}

std::array<std::size_t, NUM_BUCKETS> histogram(const std::vector<int> &values) {
  std::array<std::size_t, NUM_BUCKETS> retBuckets{0};
  for (auto v : values) {
    retBuckets[mapToBucket(v)]++;
  }
  return retBuckets;
}
