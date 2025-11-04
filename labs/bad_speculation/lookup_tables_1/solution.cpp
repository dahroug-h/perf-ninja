#include "solution.hpp"
#include <cstdint>

uint8_t buckets[100] = {0,0,0,0,0,0,0,0,0,0,0,0,0,
                              1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
                              2,2,2,2,2,2,2,2,2,2,2,2,
                              3,3,3,3,3,3,3,3,3,3,3,3,
                              4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,
                              5,5,5,5,5,5,5,5,5,5,5,5,
                              6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6};


static std::size_t mapToBucket(std::size_t v) {
                              //   size of a bucket
  // if (v < 100)
  //   return buckets[v];

  if (v < sizeof(buckets)/sizeof(uint8_t)){
    return buckets[v];
  }

  return DEFAULT_BUCKET;
}

std::array<std::size_t, NUM_BUCKETS> histogram(const std::vector<int> &values) {
  std::array<std::size_t, NUM_BUCKETS> retBuckets{0};
  for (auto v : values) {
    retBuckets[mapToBucket(v)]++;
  }
  return retBuckets;
}
