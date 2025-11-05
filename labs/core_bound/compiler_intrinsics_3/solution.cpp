#include "solution.hpp"
#include <algorithm>
#include <immintrin.h>

Position<std::uint32_t> solution(std::vector<Position<std::uint32_t>> const &input) {
  std::uint64_t x = 0;
  std::uint64_t y = 0;
  std::uint64_t z = 0;

  // Fast path: AVX2 gather-based vectorization over struct-of-3-uint32 elements.
  // Each Position has 3 uint32_t words (x,y,z) so the data layout is:
  // x0,y0,z0, x1,y1,z1, x2,y2,z2, ...
  // We'll treat the underlying memory as an array of 32-bit words and gather
  // the x/y/z components using indices multiplied by 4 (scale=4).
  size_t n = input.size();
#if defined(__AVX2__)
  if (n) {
    // Process input in small aligned blocks and de-interleave into stack
    // buffers to avoid heap allocations. We use a block size of 8 because
    // AVX2 can load 8 x u32 lanes in a 256-bit register.
    const size_t Block = 8;

    // 64-bit accumulators (two 4-lane 64-bit vectors) per component
    __m256i accx_lo = _mm256_setzero_si256();
    __m256i accx_hi = _mm256_setzero_si256();
    __m256i accy_lo = _mm256_setzero_si256();
    __m256i accy_hi = _mm256_setzero_si256();
    __m256i accz_lo = _mm256_setzero_si256();
    __m256i accz_hi = _mm256_setzero_si256();

    size_t i = 0;
    alignas(32) std::uint32_t bx[Block];
    alignas(32) std::uint32_t by[Block];
    alignas(32) std::uint32_t bz[Block];

    for (; i + Block <= n; i += Block) {
      // de-interleave into small stack buffers
      for (size_t j = 0; j < Block; ++j) {
        auto const &p = input[i + j];
        bx[j] = p.x;
        by[j] = p.y;
        bz[j] = p.z;
      }

      __m256i vx = _mm256_load_si256(reinterpret_cast<const __m256i *>(bx));
      __m256i vy = _mm256_load_si256(reinterpret_cast<const __m256i *>(by));
      __m256i vz = _mm256_load_si256(reinterpret_cast<const __m256i *>(bz));

      __m128i vx_lo128 = _mm256_castsi256_si128(vx);
      __m128i vx_hi128 = _mm256_extracti128_si256(vx, 1);
      __m256i wx_lo = _mm256_cvtepu32_epi64(vx_lo128);
      __m256i wx_hi = _mm256_cvtepu32_epi64(vx_hi128);
      accx_lo = _mm256_add_epi64(accx_lo, wx_lo);
      accx_hi = _mm256_add_epi64(accx_hi, wx_hi);

      __m128i vy_lo128 = _mm256_castsi256_si128(vy);
      __m128i vy_hi128 = _mm256_extracti128_si256(vy, 1);
      __m256i wy_lo = _mm256_cvtepu32_epi64(vy_lo128);
      __m256i wy_hi = _mm256_cvtepu32_epi64(vy_hi128);
      accy_lo = _mm256_add_epi64(accy_lo, wy_lo);
      accy_hi = _mm256_add_epi64(accy_hi, wy_hi);

      __m128i vz_lo128 = _mm256_castsi256_si128(vz);
      __m128i vz_hi128 = _mm256_extracti128_si256(vz, 1);
      __m256i wz_lo = _mm256_cvtepu32_epi64(vz_lo128);
      __m256i wz_hi = _mm256_cvtepu32_epi64(vz_hi128);
      accz_lo = _mm256_add_epi64(accz_lo, wz_lo);
      accz_hi = _mm256_add_epi64(accz_hi, wz_hi);
    }

    // reduce the 64-bit lane accumulators into scalars
    alignas(32) std::uint64_t tmp[4];
    _mm256_store_si256(reinterpret_cast<__m256i *>(tmp), accx_lo);
    x += tmp[0] + tmp[1] + tmp[2] + tmp[3];
    _mm256_store_si256(reinterpret_cast<__m256i *>(tmp), accx_hi);
    x += tmp[0] + tmp[1] + tmp[2] + tmp[3];

    _mm256_store_si256(reinterpret_cast<__m256i *>(tmp), accy_lo);
    y += tmp[0] + tmp[1] + tmp[2] + tmp[3];
    _mm256_store_si256(reinterpret_cast<__m256i *>(tmp), accy_hi);
    y += tmp[0] + tmp[1] + tmp[2] + tmp[3];

    _mm256_store_si256(reinterpret_cast<__m256i *>(tmp), accz_lo);
    z += tmp[0] + tmp[1] + tmp[2] + tmp[3];
    _mm256_store_si256(reinterpret_cast<__m256i *>(tmp), accz_hi);
    z += tmp[0] + tmp[1] + tmp[2] + tmp[3];

    // tail
    for (; i < n; ++i) {
      x += input[i].x;
      y += input[i].y;
      z += input[i].z;
    }
  }
#else
  for (auto pos: input) {
    x += pos.x;
    y += pos.y;
    z += pos.z;
  }
#endif

  return {
          static_cast<std::uint32_t>(x / std::max<std::uint64_t>(1, input.size())),
          static_cast<std::uint32_t>(y / std::max<std::uint64_t>(1, input.size())),
          static_cast<std::uint32_t>(z / std::max<std::uint64_t>(1, input.size())),
  };
}