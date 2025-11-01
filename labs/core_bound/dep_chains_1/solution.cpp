#include "solution.hpp"
#include <array>
#include <algorithm>
#include <iostream>

unsigned getSumOfDigits(unsigned n) {
  unsigned sum = 0;
  while (n != 0) {
    sum = sum + n % 10;
    n = n / 10;
  }
  return sum;
}

template <int M>
unsigned SolutionTemplate(List *l1, List *l2) {
  unsigned retVal = 0;

  List *head2 = l2;
  while (l1) {
    std::vector<unsigned> batch_values;
    std::vector<bool> batch_found;
    for (int i = 0; i < M; ++i) {
      if (l1) {
        batch_values.push_back(l1->value);
        batch_found.push_back(false);
        l1 = l1->next;
      } else {
        break;
      }
    }
    l2 = head2;
    while (l2) {
      for (int i = 0; i < batch_values.size(); i++) {
        if (l2->value == batch_values[i]) {
          retVal += getSumOfDigits(batch_values[i]);
          batch_found[i] = true;
          break;
        } 
      }
      l2 = l2->next;
    }
  }
  return retVal;
}

// Task: lookup all the values from l2 in l1.
// For every found value, find the sum of its digits.
// Return the sum of all digits in every found number.
// Both lists have no duplicates and elements placed in *random* order.
// Do NOT sort any of the lists. Do NOT store elements in a hash_map/sets.

// Hint: Traversing a linked list is a long data dependency chain:
//       to get the node N+1 you need to retrieve the node N first.
//       Think how you can execute multiple dependency chains in parallel.
unsigned solution(List *l1, List *l2) {
  return SolutionTemplate<4>(l1, l2);
}
