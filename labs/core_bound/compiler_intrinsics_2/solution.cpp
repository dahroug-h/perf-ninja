#include "solution.hpp"
#include <iostream>
#include <immintrin.h>

// Find the longest line in a file.
// Implementation uses ternary operator with a hope that compiler will
// turn it into a CMOV instruction.
// The code inside the inner loop is equivalent to:
/*
if (cur == '\n') {
  longestLine = std::max(curLineLength, longestLine);
  curLineLength = 0;
} else {
  curLineLength++;
}*/
unsigned solution(const std::string &inputContents) {
  unsigned longestLine = 0;
  unsigned curLineLength = 0;

  const __m256i mask = _mm256_set1_epi8('\n');
  const size_t size = inputContents.size();
  int idx = 0;
  for (; idx + 31 < size; idx += 32) {
    __m256i characters =  _mm256_loadu_si256(reinterpret_cast<const __m256i_u *>(&inputContents[idx]));
    __m256i result = _mm256_cmpeq_epi8(characters, mask);
    unsigned int newlines_mask = _mm256_movemask_epi8(result);

    int left = 32;
    while (newlines_mask) {
      int bit_pos = __builtin_ctz(newlines_mask);
      longestLine = std::max(curLineLength + bit_pos, longestLine);
      curLineLength = 0;
      left -= bit_pos + 1;
      if (bit_pos >= 31) break;
      newlines_mask >>= bit_pos + 1;
    }

    curLineLength += left;
  }

  for (; idx < size; idx++) {
    const char cur = inputContents[idx];
    curLineLength = (cur == '\n') ? 0 : curLineLength + 1;
    longestLine = std::max(curLineLength, longestLine);
  }

  return longestLine;
}
