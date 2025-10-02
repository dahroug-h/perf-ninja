#include <cstdint> 
#include <vector>

// Assume those constants never change
constexpr int N = 1000000;
constexpr int minRandom = 0;
constexpr int maxRandom = 100;

// FIXME: this data structure can be reduced in size
// p = padding
// old layout 40 bytes
// i i i i p*4 l l l l l l l l s s p*6 d d d d d d d d b p*7
// without data type changing, new layout 24 bytes
// l l l l l l l l d d d d d d d d i i i i s s b p
// and by observing the range is between 0 ~ 100 
// and test has a tolerance of floating point precision of 0.001
// we can just use short and float
// and then further we realize we could use the uint8_t and uint16_t to go even further
struct S {
  float d;
  uint16_t l;
  uint8_t i;
  uint8_t s;
  bool b;
  // int i;
  // long long l;
  // short s;
  // double d;
  // bool b;

  bool operator<(const S &s) const { return this->i < s.i; }
};

void init(std::vector<S> &arr);
S create_entry(int first_value, int second_value);
void solution(std::vector<S> &arr);
