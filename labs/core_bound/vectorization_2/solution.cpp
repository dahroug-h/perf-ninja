#include "solution.hpp"
#include <immintrin.h>

// 1 - original
// uint16_t checksum(const Blob &blob) {
//   uint16_t acc = 0;
//   for (auto value : blob) {
//     acc += value;
//     acc += acc < value; // add carry
//   }
//   return acc;
// }

// 2 - uint16_t -> uint32_t
uint16_t checksum(const Blob &blob) {
  uint32_t acc = 0;
  for (auto value : blob) {
    acc += value;
  }
  acc = (acc & 0xffff) + (acc >> 16);
  acc = (acc & 0xffff) + (acc >> 16);
  return acc;
}

// 3 - simd
// uint16_t checksum(const Blob &blob) {
//   __m256i sum = _mm256_set1_epi32(0);
//   __m256i mask = _mm256_set1_epi32(0xffff);

//   int64_t* data = (int64_t*)blob.data();
//   size_t count = 0;
//   while (count < blob.size()) {
//     __m256i cur = _mm256_set_epi64x(data[0], data[1], data[2], data[3]);
//     sum = _mm256_add_epi32(sum, _mm256_and_si256(_mm256_bsrli_epi128(cur, 2), mask));
//     sum = _mm256_add_epi32(sum, _mm256_and_si256(cur, mask));
//     count += 16;
//     data += 4;
//   }

//   uint32_t parts[8];
//   _mm256_store_si256((__m256i*)parts, sum);
//   uint32_t acc = 0;
//   for (int i = 0; i < 8; i++) {
//     acc += parts[i];
//   }
//   acc = (acc & 0xffff) + (acc >> 16);
//   acc = (acc & 0xffff) + (acc >> 16);
//   return acc;
// }

// 4 - other
// uint16_t checksum(const Blob &blob) {
//   std::array<uint32_t, 8> sums;
//   for (int i = 0; i < 8; i++) {
//     sums[i] = 0;
//   }
//   uint16_t* data = (uint16_t*)blob.data();
//   size_t count = 0;

//   while (count < blob.size()) {
//     sums[0] += data[0];
//     sums[1] += data[1];
//     sums[2] += data[2];
//     sums[3] += data[3];
//     sums[4] += data[4];
//     sums[5] += data[5];
//     sums[6] += data[6];
//     sums[7] += data[7];
//     data += 8;
//     count += 8;
//   }

//   uint32_t acc = 0;
//   for (int i = 0; i < 8; i++) {
//     acc += sums[i];
//   }
//   acc = (acc & 0xffff) + (acc >> 16);
//   acc = (acc & 0xffff) + (acc >> 16);
//   return acc;
// }
