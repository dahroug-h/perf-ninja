
#include "solution.h"
#include <memory>
#include <immintrin.h>
#include <iostream>

void imageSmoothing1(const InputVector &input, uint8_t radius,
                    OutputVector &output) {
  int pos = 0;
  int currentSum = 0;
  int size = static_cast<int>(input.size());

  // 1. left border - time spend in this loop can be ignored, no need to
  // optimize it
  for (int i = 0; i < std::min<int>(size, radius); ++i) {
    currentSum += input[i];
  }

  int limit = std::min(radius + 1, size - radius);
  for (pos = 0; pos < limit; ++pos) {
    currentSum += input[pos + radius];
    output[pos] = currentSum;
  }

  // 2. main loop.
  limit = size - radius;
  for (; pos < limit; ++pos) {
    currentSum -= input[pos - radius - 1];
    currentSum += input[pos + radius];
    output[pos] = currentSum;
  }

  // 3. special case, executed only if size <= 2*radius + 1
  limit = std::min(radius + 1, size);
  for (; pos < limit; pos++) {
    output[pos] = currentSum;
  }

  // 4. right border - time spend in this loop can be ignored, no need to
  // optimize it
  for (; pos < size; ++pos) {
    currentSum -= input[pos - radius - 1];
    output[pos] = currentSum;
  }
}

void imageSmoothing2(const InputVector &input, uint8_t radius,
                    OutputVector &output) {
  int pos = 0;
  int currentSum = 0;
  int size = static_cast<int>(input.size());

  // 1. left border - time spend in this loop can be ignored, no need to
  // optimize it
  for (int i = 0; i < std::min<int>(size, radius); ++i) {
    currentSum += input[i];
  }

  int limit = std::min(radius + 1, size - radius);
  for (pos = 0; pos < limit; ++pos) {
    currentSum += input[pos + radius];
    output[pos] = currentSum;
  }

  // 2. main loop.
  limit = size - radius;


  const uint8_t* sub_ptr = input.data() + pos - radius - 1;
  const uint8_t* add_ptr = input.data() + pos + radius;
  const uint16_t* output_ptr = output.data() + pos;
  __m128i cur = _mm_set1_epi16(currentSum);

  int i = 0;
  for (; i + 7 < limit - pos; i += 8) {
    __m128i sub_u8 = _mm_loadu_si64(sub_ptr + i);
    __m128i sub = _mm_cvtepu8_epi16(sub_u8);
    __m128i add_u8 = _mm_loadu_si64(add_ptr + i);
    __m128i add = _mm_cvtepu8_epi16(add_u8);
    __m128i diff = _mm_sub_epi16(add, sub);

    __m128i s = _mm_add_epi16(diff, _mm_slli_si128(diff, 2));
    s = _mm_add_epi16(s, _mm_slli_si128(s, 4));
    s = _mm_add_epi16(s, _mm_slli_si128(s, 8));

    __m128i result = _mm_add_epi16(s, cur);
    _mm_storeu_si128((__m128i*)(output_ptr + i), result);

    currentSum = (uint16_t)_mm_extract_epi16(result, 7);
    cur = _mm_set1_epi16(currentSum);
  }
  pos += i;

  for (; pos < limit; ++pos) {
    currentSum -= input[pos - radius - 1];
    currentSum += input[pos + radius];
    output[pos] = currentSum;
  }

  // 3. special case, executed only if size <= 2*radius + 1
  limit = std::min(radius + 1, size);
  for (; pos < limit; pos++) {
    output[pos] = currentSum;
  }

  // 4. right border - time spend in this loop can be ignored, no need to
  // optimize it
  for (; pos < size; ++pos) {
    currentSum -= input[pos - radius - 1];
    output[pos] = currentSum;
  }
}

void printV(const char* name, __m256i vals) {
  short* p = (short*)(&vals);
  std::cout << name;
  for (int i = 0; i < 16; i++) {
    std::cout << p[i] << " ";
  }
  std::cout << "\n";
}

void imageSmoothing3(const InputVector &input, uint8_t radius,
                    OutputVector &output) {
  int pos = 0;
  int currentSum = 0;
  int size = static_cast<int>(input.size());
  // std::cout << size << "\n";

  // 1. left border - time spend in this loop can be ignored, no need to
  // optimize it
  for (int i = 0; i < std::min<int>(size, radius); ++i) {
    currentSum += input[i];
  }

  int limit = std::min(radius + 1, size - radius);
  for (pos = 0; pos < limit; ++pos) {
    currentSum += input[pos + radius];
    output[pos] = currentSum;
  }

  // 2. main loop.
  limit = size - radius;


  const uint8_t* sub_ptr = input.data() + pos - radius - 1;
  const uint8_t* add_ptr = input.data() + pos + radius;
  const uint16_t* output_ptr = output.data() + pos;
  __m256i cur = _mm256_set1_epi16(currentSum);

  int i = 0;
  for (; i + 15 < limit - pos; i += 16) {
    __m128i sub_u8 = _mm_loadu_si128((__m128i const*)(sub_ptr + i));
    __m256i sub = _mm256_cvtepu8_epi16(sub_u8);
    __m128i add_u8 = _mm_loadu_si128((__m128i const*)(add_ptr + i));
    __m256i add = _mm256_cvtepu8_epi16(add_u8);
    __m256i diff = _mm256_sub_epi16(add, sub);

    __m256 s = _mm256_add_epi16(diff, _mm256_slli_si256(diff, 2));
    s = _mm256_add_epi16(s, _mm256_slli_si256(s, 4));
    s = _mm256_add_epi16(s, _mm256_slli_si256(s, 8));
    uint16_t f = (uint16_t)_mm256_extract_epi16(s, 7);
    s = _mm256_add_epi16(s,
      _mm256_set_epi16(f, f, f, f, f, f, f, f, 0, 0, 0, 0, 0, 0, 0, 0));

    __m256i result = _mm256_add_epi16(s, cur);
    _mm256_storeu_si256((__m256i*)(output_ptr + i), result);

    currentSum = (uint16_t)_mm256_extract_epi16(result, 15);
    cur = _mm256_set1_epi16(currentSum);

    // if (i == 0) {
    //   printV("sub: ", sub);
    //   printV("add: ", add);
    //   printV("s:   ", s);
    // }
  }
  pos += i;

  for (; pos < limit; ++pos) {
    currentSum -= input[pos - radius - 1];
    currentSum += input[pos + radius];
    output[pos] = currentSum;
  }

  // 3. special case, executed only if size <= 2*radius + 1
  limit = std::min(radius + 1, size);
  for (; pos < limit; pos++) {
    output[pos] = currentSum;
  }

  // 4. right border - time spend in this loop can be ignored, no need to
  // optimize it
  for (; pos < size; ++pos) {
    currentSum -= input[pos - radius - 1];
    output[pos] = currentSum;
  }
}

void imageSmoothing(const InputVector &input, uint8_t radius,
                    OutputVector &output) {
  return imageSmoothing3(input, radius, output);
}
