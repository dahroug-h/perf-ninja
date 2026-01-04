
#include "solution.h"
#include <algorithm>
#include <stdlib.h>

static int compare(const void *lhs, const void *rhs) {
  auto &a = *reinterpret_cast<const S *>(lhs);
  auto &b = *reinterpret_cast<const S *>(rhs);

  if (a.key1 < b.key1)
    return -1;

  if (a.key1 > b.key1)
    return 1;

  if (a.key2 < b.key2)
    return -1;

  if (a.key2 > b.key2)
    return 1;

  return 0;
}

// 1 - original - time 641
// void solution(std::array<S, N> &arr) {
//   qsort(arr.data(), arr.size(), sizeof(S), compare);
// }

// 2 - std::sort - time 427
// void solution(std::array<S, N> &arr) {
//   std::sort(arr.begin(), arr.end(), [](const S& lhs, const S& rhs) {
//     return (lhs.key1 < rhs.key1) || (lhs.key1 == rhs.key1 && lhs.key2 < rhs.key2);
//   });
// }

// 3 - S -> uin64_t  - time 348
void solution(std::array<S, N> &arr) {
  for (auto& v : arr) {
    std::swap(v.key1, v.key2);
  }
  auto& arr2 = reinterpret_cast<std::array<uint64_t, N>&>(arr);
  std::sort(arr2.begin(), arr2.end());
  for (auto& v : arr) {
    std::swap(v.key1, v.key2);
  }
}
