
#include "solution.h"
#include <algorithm>
#include <stdlib.h>

void solution(std::array<S, N> &arr) {
  auto compare = [](const S& lhs, const S& rhs) -> bool
  {
    if (lhs.key1 < rhs.key1)
      return true;
    if (lhs.key1 > rhs.key1)
      return false;

    if (lhs.key2 < rhs.key2)
      return true;
    if (lhs.key2 > rhs.key2)
      return false;    

    return false;    
  };

  std::sort(arr.begin(), arr.end(), compare);
}