#include "solution.hpp"
#include <algorithm>

#include <x86intrin.h>

Position<std::uint32_t> solution(std::vector<Position<std::uint32_t>> const &input) {
  std::uint64_t x = 0;
  std::uint64_t y = 0;
  std::uint64_t z = 0;

  if (input.size() < 4) {
    for (auto pos: input) {
      x += pos.x;
      y += pos.y;
      z += pos.z;
    }
  } else {
    size_t i = 0;
    const size_t n = (input.size() - 4) & ~(size_t)4;
    __m256i sum = _mm256_setzero_si256();
    for (; i < n; i += 4) {
      __m256i p0 = _mm256_cvtepu32_epi64(_mm_load_si128((__m128i *)&input[i]));
      __m256i p1 = _mm256_cvtepu32_epi64(_mm_load_si128((__m128i *)&input[i + 1]));
      __m256i p2 = _mm256_cvtepu32_epi64(_mm_load_si128((__m128i *)&input[i + 2]));
      __m256i p3 = _mm256_cvtepu32_epi64(_mm_load_si128((__m128i *)&input[i + 3]));

      sum = _mm256_add_epi64(sum, p0);
      sum = _mm256_add_epi64(sum, p1);
      sum = _mm256_add_epi64(sum, p2);
      sum = _mm256_add_epi64(sum, p3);
    }

    x = _mm256_extract_epi64(sum, 0);
    y = _mm256_extract_epi64(sum, 1);
    z = _mm256_extract_epi64(sum, 2);

    for (; i < input.size(); i++) {
      x += input[i].x;
      y += input[i].y;
      z += input[i].z;
    }
  }

  return {
          static_cast<std::uint32_t>(x / std::max<std::uint64_t>(1, input.size())),
          static_cast<std::uint32_t>(y / std::max<std::uint64_t>(1, input.size())),
          static_cast<std::uint32_t>(z / std::max<std::uint64_t>(1, input.size())),
  };
}