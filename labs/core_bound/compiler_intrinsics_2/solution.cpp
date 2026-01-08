#include "solution.hpp"
#include <iostream>
#include <immintrin.h>

// Find the longest line in a file.
// Implementation uses ternary operator with a hope that compiler will
// turn it into a CMOV instruction.
// The code inside the inner loop is equivalent to:
/*
if (s == '\n') {
  longestLine = std::max(curLineLength, longestLine);
  curLineLength = 0;
} else {
  curLineLength++;
}*/
unsigned solution1(const std::string &inputContents) {
  unsigned longestLine = 0;
  unsigned curLineLength = 0;

  for (auto s : inputContents) {
    curLineLength = (s == '\n') ? 0 : curLineLength + 1;
    longestLine = std::max(curLineLength, longestLine);
  }

  return longestLine;
}

unsigned solution2(const std::string &inputContents) {
  uint32_t longestLine = 0;
  uint32_t curLineLength = 0;
  __m256i slashn = _mm256_set1_epi8('\n');
  const char* buffer = inputContents.data();
  uint32_t N = inputContents.size();
  uint32_t i = 0;

  while (i + 32 < N) {
    uint32_t diff = _mm256_movemask_epi8(_mm256_cmpeq_epi8(_mm256_loadu_si256((const __m256i*)buffer), slashn));
    i += 32;
    buffer += 32;
    uint32_t blockLen = 32;
    
    while (diff != 0) {
      uint32_t pos = _tzcnt_u32(diff);
      longestLine = std::max(curLineLength + pos, longestLine);
      curLineLength = 0;
      blockLen -= (pos + 1);
      diff = (pos == 31 ? 0 : diff >> (pos + 1));
    }
    curLineLength += blockLen;
  }
  longestLine = std::max(curLineLength, longestLine);

  N -= i;
  for (i = 0; i < N; i++) {
    curLineLength = (buffer[i] == '\n') ? 0 : curLineLength + 1;
    longestLine = std::max(curLineLength, longestLine);
  }

  return longestLine;
}

unsigned solution(const std::string &inputContents) {
  return solution2(inputContents);
}
