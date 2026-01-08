#include "solution.hpp"
#include <algorithm>
#include <immintrin.h>

Position<std::uint32_t> solution1(std::vector<Position<std::uint32_t>> const &input) {
  std::uint64_t x = 0;
  std::uint64_t y = 0;
  std::uint64_t z = 0;

  for (auto pos: input) {
    x += pos.x;
    y += pos.y;
    z += pos.z;
  }

  return {
          static_cast<std::uint32_t>(x / std::max<std::uint64_t>(1, input.size())),
          static_cast<std::uint32_t>(y / std::max<std::uint64_t>(1, input.size())),
          static_cast<std::uint32_t>(z / std::max<std::uint64_t>(1, input.size())),
  };
}

Position<std::uint32_t> solution2(std::vector<Position<std::uint32_t>> const &input) {
  size_t N = input.size();
  size_t i = 0;
  const Position<std::uint32_t>* data = input.data();

  __m256i zero = _mm256_set1_epi64x(0);
  __m256i sum1 = _mm256_set1_epi64x(0);
  __m256i sum2 = _mm256_set1_epi64x(0);

  while (i + 4 <= N) {
    __m256i cur = _mm256_loadu_si256((__m256i const*)data);
    sum1 = _mm256_add_epi64(sum1, _mm256_unpacklo_epi32(cur, zero));
    sum2 = _mm256_add_epi64(sum2, _mm256_unpackhi_epi32(cur, zero));

    __m256i cur2 = _mm256_loadu_si256((__m256i const*)(data + 2));
    sum1 = _mm256_add_epi64(sum1, _mm256_unpacklo_epi32(cur2, zero));
    sum2 = _mm256_add_epi64(sum2, _mm256_unpackhi_epi32(cur2, zero));

    i += 4;
    data += 4;
  }

  uint64_t* s1 = (uint64_t*)&sum1;
  uint64_t* s2 = (uint64_t*)&sum2;

  std::uint64_t x = s1[0] + s1[2];
  std::uint64_t y = s1[1] + s1[3];
  std::uint64_t z = s2[0] + s2[2];
  while (i < N) {
    const auto& pos = *data;
    x += pos.x;
    y += pos.y;
    z += pos.z;

    i++;
    data++;
  }

  return {
          static_cast<std::uint32_t>(x / std::max<std::uint64_t>(1, input.size())),
          static_cast<std::uint32_t>(y / std::max<std::uint64_t>(1, input.size())),
          static_cast<std::uint32_t>(z / std::max<std::uint64_t>(1, input.size())),
  };
}

Position<std::uint32_t> solution(std::vector<Position<std::uint32_t>> const &input) {
  return solution2(input);
}
