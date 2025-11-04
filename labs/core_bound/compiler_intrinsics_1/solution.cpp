
#include "solution.h"
#include <memory>
#include <cstdint>
#include <vector>

#if defined(__x86_64) || defined (_M_X64)
  #include <immintrin.h>
  #define USE_X86_SIMD
#elif defined(__aarch64__) || defined(_M_ARM64)
  #include <arm_neon.h>
  #define USE_ARM_NEON
#endif

void imageSmoothing(const InputVector &input, uint8_t radius,
                    OutputVector &output) {
  int pos = 0;
  int currentSum = 0;
  int size = static_cast<int>(input.size());

  // 1. left border - time spend in this loop can be ignored, no need to
  // optimize it
  for (int i = 0; i < std::min<int>(size, radius); ++i) {
    currentSum += input[i];
  }

  int limit = std::min(radius + 1, size - radius);
  for (pos = 0; pos < limit; ++pos) {
    currentSum += input[pos + radius];
    output[pos] = currentSum;
  }

// 2. main loop.
  limit = size - radius;

  #if defined (USE_X86_SIMD) && defined (__AVX2__)
    // Process 16 elements at a time
    for(; pos + 16 <= limit; pos += 16){
      __m128i sub_u8 = _mm_loadu_si128((__m128i*)&input[pos - radius - 1]);
      __m256i sub = _mm256_cvtepu8_epi16(sub_u8);

      __m128i add_u8 = _mm_loadu_si128((__m128i*)&input[pos + radius]);
      __m256i add = _mm256_cvtepu8_epi16(add_u8);

      __m256i deltas = _mm256_sub_epi16(add, sub);

      // Horizontal prefix sum across the deltas vector
      // First, extract to two 128-bit halves
      __m128i deltas_lo = _mm256_castsi256_si128(deltas);
      __m128i deltas_hi = _mm256_extracti128_si256(deltas, 1);

      // Convert to 32-bit for accumulation
      __m128i d0_16 = deltas_lo;
      __m128i d1_16 = deltas_hi;
      
      // Unpack to get 8 int32 values from each half
      __m128i zero = _mm_setzero_si128();
      __m128i d0_lo = _mm_unpacklo_epi16(d0_16, _mm_cmpgt_epi16(zero, d0_16)); // sign extend
      __m128i d0_hi = _mm_unpackhi_epi16(d0_16, _mm_cmpgt_epi16(zero, d0_16));
      __m128i d1_lo = _mm_unpacklo_epi16(d1_16, _mm_cmpgt_epi16(zero, d1_16));
      __m128i d1_hi = _mm_unpackhi_epi16(d1_16, _mm_cmpgt_epi16(zero, d1_16));

      // Extract and accumulate (still need scalar loop for prefix sum dependency)
      alignas(16) int32_t d[16];
      _mm_store_si128((__m128i*)&d[0], d0_lo);
      _mm_store_si128((__m128i*)&d[4], d0_hi);
      _mm_store_si128((__m128i*)&d[8], d1_lo);
      _mm_store_si128((__m128i*)&d[12], d1_hi);

      // Manual unroll for better performance
      currentSum += d[0];  output[pos + 0]  = currentSum;
      currentSum += d[1];  output[pos + 1]  = currentSum;
      currentSum += d[2];  output[pos + 2]  = currentSum;
      currentSum += d[3];  output[pos + 3]  = currentSum;
      currentSum += d[4];  output[pos + 4]  = currentSum;
      currentSum += d[5];  output[pos + 5]  = currentSum;
      currentSum += d[6];  output[pos + 6]  = currentSum;
      currentSum += d[7];  output[pos + 7]  = currentSum;
      currentSum += d[8];  output[pos + 8]  = currentSum;
      currentSum += d[9];  output[pos + 9]  = currentSum;
      currentSum += d[10]; output[pos + 10] = currentSum;
      currentSum += d[11]; output[pos + 11] = currentSum;
      currentSum += d[12]; output[pos + 12] = currentSum;
      currentSum += d[13]; output[pos + 13] = currentSum;
      currentSum += d[14]; output[pos + 14] = currentSum;
      currentSum += d[15]; output[pos + 15] = currentSum;
    }

  #elif defined (USE_ARM_NEON)
    // Process 8 elements at a time
    for(; pos + 8 <= limit; pos += 8){
      uint8x8_t sub_u8 = vld1_u8(&input[pos - radius - 1]);
      int16x8_t sub = vreinterpretq_s16_u16(vmovl_u8(sub_u8));

      uint8x8_t add_u8 = vld1_u8(&input[pos + radius]);
      int16x8_t add = vreinterpretq_s16_u16(vmovl_u8(add_u8));

      int16x8_t deltas = vsubq_s16(add, sub);

      alignas(16) int16_t d[8];
      vst1q_s16(d, deltas);

      // Manual unroll
      currentSum += d[0]; output[pos + 0] = currentSum;
      currentSum += d[1]; output[pos + 1] = currentSum;
      currentSum += d[2]; output[pos + 2] = currentSum;
      currentSum += d[3]; output[pos + 3] = currentSum;
      currentSum += d[4]; output[pos + 4] = currentSum;
      currentSum += d[5]; output[pos + 5] = currentSum;
      currentSum += d[6]; output[pos + 6] = currentSum;
      currentSum += d[7]; output[pos + 7] = currentSum;
    }
  #endif

  for (; pos < limit; ++pos) {
    currentSum += input[pos + radius] - input[pos - radius - 1];
    output[pos] = currentSum;
  }

  // 3. special case, executed only if size <= 2*radius + 1
  limit = std::min(radius + 1, size);
  for (; pos < limit; pos++) {
    output[pos] = currentSum;
  }

  // 4. right border - time spend in this loop can be ignored, no need to
  // optimize it
  for (; pos < size; ++pos) {
    currentSum -= input[pos - radius - 1];
    output[pos] = currentSum;
  }
}
