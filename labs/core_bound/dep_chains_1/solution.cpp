#include "solution.hpp"
#define SOLUTION
#define MULTI_OVERLAP

#include <array>
#include <iostream>

unsigned getSumOfDigits(unsigned n) {
  unsigned sum = 0;
  while (n != 0) {
    sum = sum + n % 10;
    n = n / 10;
  }
  return sum;
}

  // Task: lookup all the values from l2 in l1.
  // For every found value, find the sum of its digits.
  // Return the sum of all digits in every found number.
  // Both lists have no duplicates and elements placed in *random* order.
  // Do NOT sort any of the lists. Do NOT store elements in a hash_map/sets.

  // Hint: Traversing a linked list is a long data dependency chain:
  //       to get the node N+1 you need to retrieve the node N first.
  //       Think how you can execute multiple dependency chains in parallel.

  constexpr int num_partitions = 128;
  constexpr int num_cached = 4;
#ifndef SOLUTION
  unsigned solution(List* l1, List* l2) {
    unsigned retVal = 0;

    List* head2 = l2;
    while (l1) {
      unsigned v = l1->value;
      l2 = head2;
      while (l2) {
        if (l2->value == v) {
          retVal += getSumOfDigits(v);
          break;
        }
        l2 = l2->next;
      }
      l1 = l1->next;
    }

    return retVal;
  }
#else

  unsigned solution(List* l1, List* l2) {
    unsigned retVal = 0;
    unsigned cache[num_cached];
    List* head2 = l2;
    while (l1) {
      int cnt = 0;
      while (l1 && cnt < num_cached) {
        cache[cnt] = l1->value;
        cnt++;
        l1 = l1->next;
      }

      l2 = head2;
      while (l2) {
        unsigned v = l2->value;
        l2 = l2->next;

        for (int i = 0; i < cnt; i++) {
          if (v == cache[i]) {
            retVal += getSumOfDigits(cache[i]);
            break;
          }
        }
      }
    }

    return retVal;
  }

#endif
