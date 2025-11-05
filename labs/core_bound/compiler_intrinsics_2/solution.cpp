#include "solution.hpp"

#if defined(__x86_64__) || defined(_M_X64) || defined(__i386__) || defined(_M_IX86)
#include <emmintrin.h> // SSE2
#define USE_X86_INTRINSICS
#elif defined(__ARM_NEON) || defined(__aarch64__)
#include <arm_neon.h>
#define USE_ARM_NEON
#endif

// Find the longest line in a file using SIMD intrinsics
unsigned solution_intrinsics(const std::string &inputContents) {
  const char *data = inputContents.data();
  size_t size = inputContents.size();
  unsigned longestLine = 0;
  unsigned curLineLength = 0;
  
  size_t i = 0;
  
#ifdef USE_X86_INTRINSICS
  // Process 16 bytes at a time using SSE2
  const size_t simd_width = 16;
  const __m128i newline_vec = _mm_set1_epi8('\n'); // creating mask with eol character
  
  while (i + simd_width <= size) {
    // Load 16 characters
    __m128i chars = _mm_loadu_si128(reinterpret_cast<const __m128i*>(data + i)); // loads chunk of text
    
    // Compare with newline character
    __m128i cmp = _mm_cmpeq_epi8(chars, newline_vec); // vector compare
    int mask = _mm_movemask_epi8(cmp);
    
    if (mask == 0) {
      // No newlines in this chunk
      curLineLength += simd_width;
      i += simd_width;
    } else {
      // Process each byte in the chunk
      for (size_t j = 0; j < simd_width; ++j) {
        if (data[i] == '\n') {
          longestLine = (curLineLength > longestLine) ? curLineLength : longestLine;
          curLineLength = 0;
        } else {
          curLineLength++;
        }
        i++;
      }
    }
  }
  
#elif defined(USE_ARM_NEON)
  // Process 16 bytes at a time using NEON
  const size_t simd_width = 16;
  const uint8x16_t newline_vec = vdupq_n_u8('\n');
  
  while (i + simd_width <= size) {
    // Load 16 characters
    uint8x16_t chars = vld1q_u8(reinterpret_cast<const uint8_t*>(data + i));
    
    // Compare with newline character
    uint8x16_t cmp = vceqq_u8(chars, newline_vec);
    
    // Check if any newlines found
    uint64x2_t cmp64 = vreinterpretq_u64_u8(cmp);
    uint64_t cmp_or = vgetq_lane_u64(cmp64, 0) | vgetq_lane_u64(cmp64, 1);
    
    if (cmp_or == 0) {
      // No newlines in this chunk
      curLineLength += simd_width;
      i += simd_width;
    } else {
      // Process each byte in the chunk
      for (size_t j = 0; j < simd_width; ++j) {
        if (data[i] == '\n') {
          longestLine = (curLineLength > longestLine) ? curLineLength : longestLine;
          curLineLength = 0;
        } else {
          curLineLength++;
        }
        i++;
      }
    }
  }
#endif
  
  // Process remaining bytes
  while (i < size) {
    if (data[i] == '\n') {
      longestLine = (curLineLength > longestLine) ? curLineLength : longestLine;
      curLineLength = 0;
    } else {
      curLineLength++;
    }
    i++;
  }
  
  // Handle final line if it doesn't end with newline
  longestLine = (curLineLength > longestLine) ? curLineLength : longestLine;
  
  return longestLine;
}

// Original implementation kept for reference
unsigned solution(const std::string &inputContents) {
  // unsigned longestLine = 0;
  // unsigned curLineLength = 0;

  // for (auto s : inputContents) {
  //   curLineLength = (s == '\n') ? 0 : curLineLength + 1;
  //   longestLine = std::max(curLineLength, longestLine);
  // }

  return solution_intrinsics(inputContents);
}
