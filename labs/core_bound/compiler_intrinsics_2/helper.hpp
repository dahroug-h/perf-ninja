#pragma once

#include <cstdint>
#include <cassert>
#include <emmintrin.h> // SSE2: _mm_cmpeq_epi8, _mm_movemask_epi8, _mm_cvtsi64_si128

uint8_t tzcnt(uint8_t mask) {
  // count the number of trailing zero bits in the mask.
  return __builtin_ctz(mask);
}

uint8_t compareBytes(uint64_t a, uint64_t b) {
  // perform a byte-wise comparison of a and b
  // produce a bit mask with the result of the comparison
  // one if bytes are equal, zero if different
  __m128i va =  _mm_cvtsi64_si128(a); // upper 64 bit is zeroed out
  __m128i vb =  _mm_cvtsi64_si128(b); // upper 64 bit is zeroed out
  __m128i cmp = _mm_cmpeq_epi8(va, vb); // Compare packed 8-bit integers in a and b for equality
  uint8_t mask = 0xFF & _mm_movemask_epi8(cmp); // Create mask from the most significant bit of each 8-bit element
  return mask;
}