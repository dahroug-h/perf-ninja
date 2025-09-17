#include "solution.hpp"
#include <algorithm>
#include <emmintrin.h>
#include <immintrin.h>


Position<std::uint32_t> solution(std::vector<Position<std::uint32_t>> const &input) {
  size_t idx = 0;
  __m256i aggregation1a = _mm256_set1_epi64x(0);
  __m256i aggregation2a = _mm256_set1_epi64x(0);
  __m256i aggregation3a = _mm256_set1_epi64x(0);
  __m256i aggregation1b = _mm256_set1_epi64x(0);
  __m256i aggregation2b = _mm256_set1_epi64x(0);
  __m256i aggregation3b = _mm256_set1_epi64x(0);

  for (; idx + 7 < input.size(); idx += 8) {
    // BLOCK A
    __m128i data1a = _mm_loadu_si128(reinterpret_cast<const __m128i_u *>(&input[idx]));
    __m128i data2a = _mm_loadu_si128(reinterpret_cast<const __m128i_u *>(&input[idx+1].y));
    __m128i data3a = _mm_loadu_si128(reinterpret_cast<const __m128i_u *>(&input[idx+2].z));

    __m256i extended1a = _mm256_cvtepu32_epi64(data1a);
    __m256i extended2a = _mm256_cvtepu32_epi64(data2a);
    __m256i extended3a = _mm256_cvtepu32_epi64(data3a);

    aggregation1a = _mm256_add_epi64(aggregation1a, extended1a);
    aggregation2a = _mm256_add_epi64(aggregation2a, extended2a);
    aggregation3a = _mm256_add_epi64(aggregation3a, extended3a);

    // BLOCK B
    __m128i data1b = _mm_loadu_si128(reinterpret_cast<const __m128i_u *>(&input[idx+4]));
    __m128i data2b = _mm_loadu_si128(reinterpret_cast<const __m128i_u *>(&input[idx+5].y));
    __m128i data3b = _mm_loadu_si128(reinterpret_cast<const __m128i_u *>(&input[idx+6].z));

    __m256i extended1b = _mm256_cvtepu32_epi64(data1b);
    __m256i extended2b = _mm256_cvtepu32_epi64(data2b);
    __m256i extended3b = _mm256_cvtepu32_epi64(data3b);

    aggregation1b = _mm256_add_epi64(aggregation1b, extended1b);
    aggregation2b = _mm256_add_epi64(aggregation2b, extended2b);
    aggregation3b = _mm256_add_epi64(aggregation3b, extended3b);
  }

  __m256i aggregation1 = _mm256_add_epi64(aggregation1a, aggregation1b);
  __m256i aggregation2 = _mm256_add_epi64(aggregation2a, aggregation2b);
  __m256i aggregation3 = _mm256_add_epi64(aggregation3a, aggregation3b);

  std::uint64_t x = _mm256_extract_epi64(aggregation1, 0) + _mm256_extract_epi64(aggregation1, 3) + _mm256_extract_epi64(aggregation2, 2) + _mm256_extract_epi64(aggregation3, 1);
  std::uint64_t y = _mm256_extract_epi64(aggregation1, 1) + _mm256_extract_epi64(aggregation2, 0) + _mm256_extract_epi64(aggregation2, 3) + _mm256_extract_epi64(aggregation3, 2);
  std::uint64_t z = _mm256_extract_epi64(aggregation1, 2) + _mm256_extract_epi64(aggregation2, 1) + _mm256_extract_epi64(aggregation3, 0) + _mm256_extract_epi64(aggregation3, 3);

  for (; idx < input.size(); idx++) {
    const auto& pos = input[idx];
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