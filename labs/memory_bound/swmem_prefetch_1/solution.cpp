#include "solution.hpp"

static int getSumOfDigits(int n) {
  int sum = 0;
  while (n != 0) {
    sum = sum + n % 10;
    n = n / 10;
  }
  return sum;
}

int solution(const hash_map_t *hash_map, const std::vector<int> &lookups) {
  int result = 0;

  for (size_t i = 0; i < lookups.size() - 1; i++) {
    const int val = lookups[i];
    if (hash_map->find(val))
      result += getSumOfDigits(val);
    hash_map->prefetch(lookups[(i + 16) % lookups.size()]);
  }

  const int val = lookups[lookups.size() - 1];
  if (hash_map->find(val))
    result += getSumOfDigits(val);

  return result;
}
