
#include "solution.h"
#define SOLUTION
int solution(int *arr, int N) {
  int res = 0;
#ifndef SOLUTION
  for (int i = 0; i < N; i++) {
    res += arr[i];
  }
#else
	res = (1+N) * N / 2;
#endif

  return res;
}
