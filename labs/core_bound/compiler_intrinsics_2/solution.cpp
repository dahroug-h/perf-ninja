#include "solution.hpp"
#include <iostream>

#include <x86intrin.h>

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
unsigned solution(const std::string &inputContents) {
  unsigned longestLine = 0;
  unsigned curLineLength = 0;

  const __m256i pattern = _mm256_set1_epi8('\n');
  const char* buffer = inputContents.c_str();
  const size_t length = inputContents.length() & ~(size_t)31;
  size_t i = 0;
  for (; i < length; i += 32) {
    __m256i chunk = _mm256_load_si256((__m256i *)(buffer + i));
    int mask = _mm256_movemask_epi8(_mm256_cmpeq_epi8(chunk, pattern));
    if (mask == 0) {
      longestLine = std::max(curLineLength, longestLine);
      curLineLength += 32;      
    } else {
      int k = 0;
      while (k < 32) {
        longestLine = std::max(curLineLength, longestLine);
        curLineLength = (mask & 0x1) ? 0 : (curLineLength + 1);
        k += 1;
        mask >>= 1;
      }            
    }    
  }

  for (; i < inputContents.length(); i++) {
    curLineLength = (inputContents[i] == '\n') ? 0 : curLineLength + 1;
    longestLine = std::max(curLineLength, longestLine);
  }
  return longestLine;
}
