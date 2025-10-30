#include "solution.hpp"

uint16_t checksum(const Blob &blob) {
  uint64_t acc = 0;
  for (auto value : blob) {
    acc += value;
  }
  return (acc & 0xfffff) + (acc >> 16);
}
