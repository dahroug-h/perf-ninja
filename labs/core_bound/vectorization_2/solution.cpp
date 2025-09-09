#include "solution.hpp"

uint16_t checksum(const Blob &blob) {
  uint32_t acc = 0;
  for (auto value : blob) {
    acc += value;
    // acc += acc < value; // add carry
  }
  
  while (acc > 0xFFFF) {
    uint32_t lower = acc & 0xFFFF;
    uint32_t higher = acc >> 16;
    acc = higher + lower;
  }
  return acc & 0xFFFF;
}
