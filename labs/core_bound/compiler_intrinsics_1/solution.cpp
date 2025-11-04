
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
    // Process 8 elements at a time (reduced from 16 for better performance)
    for(; pos + 8 <= limit; pos += 8){
      // Load and convert to int16
      __m128i sub_u8 = _mm_loadl_epi64((__m128i*)&input[pos - radius - 1]);
      __m128i sub = _mm_cvtepu8_epi16(sub_u8);

      __m128i add_u8 = _mm_loadl_epi64((__m128i*)&input[pos + radius]);
      __m128i add = _mm_cvtepu8_epi16(add_u8);

      __m128i deltas = _mm_sub_epi16(add, sub);

      // Convert to 32-bit for safe accumulation
      __m128i zero = _mm_setzero_si128();
      __m128i mask = _mm_cmpgt_epi16(zero, deltas);
      __m128i deltas_lo = _mm_unpacklo_epi16(deltas, mask);
      __m128i deltas_hi = _mm_unpackhi_epi16(deltas, mask);

      // Parallel prefix sum (scan) within vector
      // This computes cumulative sums: [d0, d0+d1, d0+d1+d2, d0+d1+d2+d3]
      __m128i sum_lo = deltas_lo;
      __m128i sum_hi = deltas_hi;
      
      // Prefix sum using shift and add
      __m128i temp;
      temp = _mm_slli_si128(sum_lo, 4);
      sum_lo = _mm_add_epi32(sum_lo, temp);
      temp = _mm_slli_si128(sum_lo, 8);
      sum_lo = _mm_add_epi32(sum_lo, temp);
      
      temp = _mm_slli_si128(sum_hi, 4);
      sum_hi = _mm_add_epi32(sum_hi, temp);
      temp = _mm_slli_si128(sum_hi, 8);
      sum_hi = _mm_add_epi32(sum_hi, temp);
      
      // Add the last element of sum_lo to all elements of sum_hi
      __m128i carry = _mm_shuffle_epi32(sum_lo, _MM_SHUFFLE(3,3,3,3));
      sum_hi = _mm_add_epi32(sum_hi, carry);
      
      // Add currentSum to all elements
      __m128i base = _mm_set1_epi32(currentSum);
      sum_lo = _mm_add_epi32(sum_lo, base);
      sum_hi = _mm_add_epi32(sum_hi, base);
      
      // Pack back to 16-bit and store
      __m128i result = _mm_packus_epi32(sum_lo, sum_hi);
      _mm_storeu_si128((__m128i*)&output[pos], result);
      
      // Update currentSum for next iteration
      alignas(16) int32_t last[4];
      _mm_store_si128((__m128i*)last, sum_hi);
      currentSum = last[3];
    }

  #elif defined (USE_ARM_NEON)
    // Process 8 elements at a time with parallel prefix sum
    for(; pos + 8 <= limit; pos += 8){
      uint8x8_t sub_u8 = vld1_u8(&input[pos - radius - 1]);
      int16x8_t sub = vreinterpretq_s16_u16(vmovl_u8(sub_u8));

      uint8x8_t add_u8 = vld1_u8(&input[pos + radius]);
      int16x8_t add = vreinterpretq_s16_u16(vmovl_u8(add_u8));

      int16x8_t deltas = vsubq_s16(add, sub);

      // Convert to 32-bit
      int32x4_t deltas_lo = vmovl_s16(vget_low_s16(deltas));
      int32x4_t deltas_hi = vmovl_s16(vget_high_s16(deltas));

      // Parallel prefix sum using pairwise adds
      // Step 1: [d0, d0+d1, d2, d2+d3]
      int32x4_t sum_lo = deltas_lo;
      int32x4_t sum_hi = deltas_hi;
      
      sum_lo = vaddq_s32(sum_lo, vextq_s32(vdupq_n_s32(0), sum_lo, 3));
      sum_lo = vaddq_s32(sum_lo, vextq_s32(vdupq_n_s32(0), sum_lo, 2));
      
      sum_hi = vaddq_s32(sum_hi, vextq_s32(vdupq_n_s32(0), sum_hi, 3));
      sum_hi = vaddq_s32(sum_hi, vextq_s32(vdupq_n_s32(0), sum_hi, 2));
      
      // Add carry from sum_lo to sum_hi
      int32_t carry = vgetq_lane_s32(sum_lo, 3);
      sum_hi = vaddq_s32(sum_hi, vdupq_n_s32(carry));
      
      // Add currentSum base
      sum_lo = vaddq_s32(sum_lo, vdupq_n_s32(currentSum));
      sum_hi = vaddq_s32(sum_hi, vdupq_n_s32(currentSum));
      
      // Pack and store
      uint16x4_t result_lo = vqmovun_s32(sum_lo);
      uint16x4_t result_hi = vqmovun_s32(sum_hi);
      vst1_u16(&output[pos], result_lo);
      vst1_u16(&output[pos + 4], result_hi);
      
      // Update currentSum
      currentSum = vgetq_lane_s32(sum_hi, 3);
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
