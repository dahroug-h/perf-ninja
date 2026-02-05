#include <vector>

// Assume those constants never change
constexpr int N = 1000000;
constexpr int minRandom = 0;
constexpr int maxRandom = 100;

struct S {
  // int i;
  // long long l;
  // short s;
  // double d;
  // bool b;

  // SOLUTION:
  // Used smaller types using known value ranges
  // Bit fields where necessary
  // Eliminated padding by reordering members (largest to smallest)
  // Float instead of double for d, as the precision is not critical

  float d;
  uint16_t l;
  uint8_t i;
  uint8_t s: 7;
  uint8_t b : 1;

  bool operator<(const S &s) const { return this->i < s.i; }
};

void init(std::vector<S> &arr);
S create_entry(int first_value, int second_value);
void solution(std::vector<S> &arr);
