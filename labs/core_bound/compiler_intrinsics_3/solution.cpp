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

  // std::cout << data << " " << data+1 << " " << N << std::endl;
  std::uint64_t x = 0;
  std::uint64_t y = 0;
  std::uint64_t z = 0;

  if ((uint64_t)data % 32 != 0 && N != 0) {
    // std::cout << "\tshift: " << ((uint64_t)data % 64) << std::endl;
    x = data[0].x; 
    y = data[0].y;
    z = data[0].z;
    i++;
    data++;
  }

  while (i + 8 <= N) {
    {
      __m256i cur = _mm256_load_si256((__m256i const*)data);
      sum1 = _mm256_add_epi64(sum1, _mm256_unpacklo_epi32(cur, zero));
      sum2 = _mm256_add_epi64(sum2, _mm256_unpackhi_epi32(cur, zero));
    }

    {
      __m256i cur = _mm256_loadu_si256((__m256i const*)(data + 2));
      sum1 = _mm256_add_epi64(sum1, _mm256_unpacklo_epi32(cur, zero));
      sum2 = _mm256_add_epi64(sum2, _mm256_unpackhi_epi32(cur, zero));
    }

    {
      __m256i cur = _mm256_loadu_si256((__m256i const*)(data + 4));
      sum1 = _mm256_add_epi64(sum1, _mm256_unpacklo_epi32(cur, zero));
      sum2 = _mm256_add_epi64(sum2, _mm256_unpackhi_epi32(cur, zero));
    }

    {
      __m256i cur = _mm256_loadu_si256((__m256i const*)(data + 6));
      sum1 = _mm256_add_epi64(sum1, _mm256_unpacklo_epi32(cur, zero));
      sum2 = _mm256_add_epi64(sum2, _mm256_unpackhi_epi32(cur, zero));
    }

    i += 8;
    data += 8;
  }

  uint64_t* s1 = (uint64_t*)&sum1;
  uint64_t* s2 = (uint64_t*)&sum2;

  x += s1[0] + s1[2];
  y += s1[1] + s1[3];
  z += s2[0] + s2[2];
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
