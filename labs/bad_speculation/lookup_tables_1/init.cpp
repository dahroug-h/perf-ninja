#include "solution.hpp"
#include <limits>
#include <random>

std::size_t buckets[151];

void init(std::vector<int> &values) {
  std::random_device r;
  std::default_random_engine generator(r());
  // generate random integer in the closed interval [0,150]
  // the chance of selecting the default bucket is ~33%
  std::uniform_int_distribution<int> distribution(0, 150);
  for (int i = 0; i < NUM_VALUES; i++) {
    values.push_back(distribution(generator));
  }
  for (int v = 0; v < sizeof(buckets)/sizeof(buckets[0]); v++) {
    if      (v < 13)  buckets[v] = 0; //   13
    else if (v < 29)  buckets[v] = 1; //   16
    else if (v < 41)  buckets[v] = 2; //   12
    else if (v < 53)  buckets[v] = 3; //   12
    else if (v < 71)  buckets[v] = 4; //   18
    else if (v < 83)  buckets[v] = 5; //   12
    else if (v < 100) buckets[v] = 6; //   17
    else buckets[v] = DEFAULT_BUCKET;    
  }
}