
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
    for(; pos + 16 <= limit; pos+=16){
      __m128i sub_u8 = _mm_loadu_si128((__m128i*)&input[pos - radius - 1]);
      __m256i sub = _mm256_cvtepu8_epi16(sub_u8);

      __m128i add_u8 = _mm_loadu_si128((__m128i*)&input[pos + radius]);
      __m256i add = _mm256_cvtepu8_epi16(add_u8);

      __m256i deltas = _mm256_sub_epi16(add, sub);

      alignas(32) int16_t delta_array[16];
      _mm256_store_si256((__m256i*)delta_array, deltas);

      for(int i = 0; i < 16; i++){
        currentSum += delta_array[i];
        output[pos + i] = currentSum;
      }
    }

  #elif defined (USE_ARM_NEON)
    for(; pos + 8 <= limit; pos+=8){
      uint8x8_t sub_u8 = vld1_u8(&input[pos - radius - 1]);
      uint16x8_t sub = vmovl_u8(sub_u8);

      uint8x8_t add_u8 = vld1_u8(&input[pos + radius]);
      uint16x8_t add = vmovl_u8(add_u8);

      uint16x8_t deltas = vreinterpretq_s16_u16(vsubq_u16(add, sub));

      alignas(16) uint16_t delta_array[8];
      vst1q_u16(delta_array, deltas);

      for(int i = 0; i < 8; i++){
        currentSum += delta_array[i];
        output[pos + i] = currentSum;
      }
    }
  #endif

  for (; pos < limit; ++pos) {
    currentSum -= input[pos - radius - 1];
    currentSum += input[pos + radius];
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
