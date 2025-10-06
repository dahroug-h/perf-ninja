#include "solution.hpp"

static int getSumOfDigits(int n) {
  int sum = 0;
  while (n != 0) {
    sum = sum + n % 10;
    n = n / 10;
  }
  return sum;
}

constexpr int lookahead = 8;
int solution(const hash_map_t *hash_map, const std::vector<int> &lookups) {
  int result = 0;
  if (lookups.empty()) {
    return result;
  }

  for (int i = 0; i < lookups.size() - lookahead; i++) {
    int val = lookups[i];
    hash_map->prefetch(lookups[i+lookahead]);
    if (hash_map->find(val)) {
      result += getSumOfDigits(val);
    }
  }
  for (int i = lookups.size() - lookahead; i < lookups.size(); i++) {
    int val = lookups[i];
    if (hash_map->find(val)) {
      result += getSumOfDigits(val);
    }
  }
  return result;
}
