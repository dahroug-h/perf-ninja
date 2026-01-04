#include "solution.hpp"
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
unsigned solution1(List *l1, List *l2) {
  unsigned retVal = 0;

  List *head2 = l2;
  // O(N^2) algorithm:
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

unsigned solution2(List *l1, List *l2) {
  unsigned retVal = 0;

  static constexpr size_t M = 16;
  std::array<unsigned, M> values;

  while (l1) {
    size_t count = 0;
    while (l1 && count < M) {
      values[count++] = l1->value;
      l1 = l1->next;
    }

    List* cur = l2;
    while (cur && count != 0) {
      unsigned v = cur->value;
      for (size_t i = 0; i < count; i++) {
        if (v == values[i]) {
          retVal += getSumOfDigits(v);
          std::swap(values[i], values[--count]);
          break;
        }
      }
      cur = cur->next;
    }
  }

  return retVal;
}

unsigned solution(List *l1, List *l2) {
  return solution2(l1, l2);
}
