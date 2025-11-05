#include "solution.hpp"
#include <algorithm>

Position<std::uint32_t> solution(std::vector<Position<std::uint32_t>> const &input) {
  std::uint64_t x = 0;
  std::uint64_t y = 0;
  std::uint64_t z = 0;

  size_t n = input.size();
  if (n == 0) {
    return {0u, 0u, 0u};
  }

  // Transform AoS -> SoA into PositionVector to get contiguous per-component
  // arrays. We resize up-front and fill by index so we do a single pass over
  // the input (no push_back overhead) and sum as we go.
  PositionVector<std::uint32_t> pv;
  pv.x.resize(n);
  pv.y.resize(n);
  pv.z.resize(n);

  for (size_t i = 0; i < n; ++i) {
    auto const &p = input[i];
    pv.x[i] = p.x;
    pv.y[i] = p.y;
    pv.z[i] = p.z;
  }

  for(size_t i = 0; i < n; ++i) {
    x += pv.x[i];
    y += pv.y[i];
    z += pv.z[i];
  }

  return {
          static_cast<std::uint32_t>(x / std::max<std::uint64_t>(1, input.size())),
          static_cast<std::uint32_t>(y / std::max<std::uint64_t>(1, input.size())),
          static_cast<std::uint32_t>(z / std::max<std::uint64_t>(1, input.size())),
  };
}