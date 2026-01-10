#include "solution.hpp"

static int getSumOfDigits(int n) {
  int sum = 0;
  while (n != 0) {
    sum = sum + n % 10;
    n = n / 10;
  }
  return sum;
}

int solution1(const hash_map_t *hash_map, const std::vector<int> &lookups) {
  int result = 0;

  for (int val : lookups) {
    if (hash_map->find(val))
      result += getSumOfDigits(val);
  }

  return result;
}

int solution2(const hash_map_t *hash_map, const std::vector<int> &lookups) {
  int result = 0;
  static constexpr int shift = 32;

  int size = lookups.size() - shift;
  for (int i = 0; i < size; i++) {
    hash_map->prefetchValue(lookups[i + shift]);
    int val = lookups[i];
    if (hash_map->find2(val))
      result += getSumOfDigits(val);
  }

  for (int i = size; i < lookups.size(); i++) {
    int val = lookups[i];
    if (hash_map->find2(val))
      result += getSumOfDigits(val);
  }

  return result;
}

int solution(const hash_map_t *hash_map, const std::vector<int> &lookups) {
  return solution2(hash_map, lookups);
}
