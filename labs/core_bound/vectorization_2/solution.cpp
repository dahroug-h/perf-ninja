#include "solution.hpp"

uint16_t checksum(const Blob &blob) {
  uint64_t acc = 0;
  for (auto value : blob) {
    acc += value;
  }
  acc = (acc & 0xffff) + (acc >> 16);
  acc = (acc & 0xffff) + (acc >> 16); // in case the final add overflow
  return acc;
}
