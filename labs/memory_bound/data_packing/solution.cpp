#include "solution.h"
#include <algorithm>
#include <array>
#include <random>

void solution(std::vector<S> &arr) {
  // 1. shuffle
  static std::random_device rd;
  static std::mt19937 g(rd());
  std::shuffle(arr.begin(), arr.end(), g);

  // 2. counting sort
  constexpr int cntSize = maxRandom - minRandom + 1;  // 101
  std::array<int, cntSize> cnt{};
  for (int k = 0; k < arr.size(); k += 4) {
    ++cnt[arr[k].i - minRandom + 1];
    ++cnt[arr[k + 1].i - minRandom + 1];
    ++cnt[arr[k + 2].i - minRandom + 1];
    ++cnt[arr[k + 3].i - minRandom + 1];
  }
  for (int k = arr.size() / 4 * 4; k < arr.size(); ++k) {
    ++cnt[arr[k].i - minRandom + 1];
  }
  // for (const auto& v : arr) {
  //   ++cnt[v.i - minRandom + 1];
  // }
  for (int i = 1; i < cntSize; ++i) {
    cnt[i] += cnt[i - 1];
  }
  std::vector<S> sorted(N);
  for (const auto& v : arr) {
    sorted[cnt[v.i - minRandom]++] = v;
  }
  arr.swap(sorted);
  // std::sort(arr.begin(), arr.end());
}
