#include "solution.hpp"
#include <iostream>
#include "helper.hpp"
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
  constexpr uint64_t EOL = 0x0A0A0A0A0A0A0A0A;
  auto *buf = inputContents.data();
  uint32_t lineBeginPos = 0;
  uint32_t pos = 0;;
  for (; pos + 7 < inputContents.size(); pos += 8) {
    // std::cout << "Debug: " << "in vectorized loop pos = " << pos << std::endl;
    // load 8 byte chunk of input string into memory
    uint64_t vect = *((const uint64_t *)(buf + pos));
    // check if any characters in this chunk is '\n' new liner
    uint8_t mask = compareBytes(vect, EOL);
    uint32_t offset = 0;
    // std::cout << "Debug: " << "mask : " << mask << std::endl;
    while (mask) {
      uint16_t localEolPos = tzcnt(mask);
      uint16_t eolPos = offset + localEolPos;
      // compute the length of the current string
      curLineLength = (pos + eolPos) - lineBeginPos;
      longestLine = std::max(longestLine, curLineLength);
      // new line starts after the '\n'
      lineBeginPos = pos + eolPos + 1;
      offset = eolPos + 1;
      if (offset >= 8) {
        break;
      } else {
        mask >>= (localEolPos + 1);
      }
    }
  }

  for (; pos < inputContents.size(); pos++) {
    if (inputContents[pos] == '\n') {
      unsigned segmentLength = pos - lineBeginPos;
      longestLine = std::max(longestLine, segmentLength);
      lineBeginPos = pos + 1;
      }
  }

  // Handle the last line if the file doesn't end with '\n'
  if (lineBeginPos < inputContents.size()) {
    unsigned segmentLength = inputContents.size() - lineBeginPos;
    longestLine = std::max(longestLine, segmentLength);
  }

  return longestLine;
}