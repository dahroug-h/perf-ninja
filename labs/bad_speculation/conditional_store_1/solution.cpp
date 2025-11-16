
#include "solution.h"

// Select items which have S.first in range [lower..upper]
std::size_t select(std::array<S, N> &output, const std::array<S, N> &input,
                   const std::uint32_t lower, const std::uint32_t upper) {
  std::size_t count = 0;
  for (const auto item : input) {
    const bool inbounds = (lower <= item.first) && (item.first <= upper);
    const auto curr = output[count];
    output[count] = __builtin_unpredictable(inbounds) ? item : curr;
    count = __builtin_unpredictable(inbounds) ? (count + 1) : count;    
  }
  return count;
}
