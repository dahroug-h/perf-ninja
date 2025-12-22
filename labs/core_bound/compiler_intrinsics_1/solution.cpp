
#include "solution.h"
#include <memory>

#include <x86intrin.h>

void imageSmoothing(const InputVector &input, uint8_t radius,
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
  const uint8_t* inLeft = NULL;
  const uint8_t* inRight = NULL;
  uint16_t* out = output.data();

  for (; (pos + 8) < limit; pos += 8) {
    __m128i sum = _mm_set1_epi16(currentSum);
    inLeft = input.data() + pos - radius - 1;
    inRight = input.data() + pos + radius;
    __m128i a = _mm_cvtepu8_epi16(_mm_loadu_si64(inLeft));
    __m128i b = _mm_cvtepu8_epi16(_mm_loadu_si64(inRight));

    __m128i d = _mm_sub_epi16(b, a);
    __m128i s = _mm_add_epi16(d, _mm_slli_si128(d, 2));
    s = _mm_add_epi16(s, _mm_slli_si128(s, 4));
    s = _mm_add_epi16(s, _mm_slli_si128(s, 8));
    sum = _mm_add_epi16(sum, s);       
    _mm_storeu_si128((__m128i *)(out + pos), sum);
    currentSum = _mm_extract_epi16(sum, 7);
  }

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
