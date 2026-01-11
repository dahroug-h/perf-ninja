#include "solution.hpp"
#define SOLUTION
#define MULTI_OVERLAP

#include <array>
#include <iostream>
// update dep_chain
unsigned getSumOfDigits(unsigned n) {
  unsigned sum = 0;
  while (n != 0) {
    sum = sum + n % 10;
    n = n / 10;
  }
  return sum;
}

#define DO()                                              \
  do {                                                    \
    if (p[i] != nullptr) {                                \
      processed++;                                        \
      if (p[i]->value == v) {                             \
        retVal += getSumOfDigits(v);                      \
      }                                                   \
                                                          \
      if (i + 1 < num_partitions &&      \
                           heads[i + 1] == p[i]->next) { \
        p[i] = nullptr;                                   \
      } else {                                            \
        p[i] = p[i]->next;                                \
      }                                                   \
    }                                                     \
  } while (0)

  // Task: lookup all the values from l2 in l1.
  // For every found value, find the sum of its digits.
  // Return the sum of all digits in every found number.
  // Both lists have no duplicates and elements placed in *random* order.
  // Do NOT sort any of the lists. Do NOT store elements in a hash_map/sets.

  // Hint: Traversing a linked list is a long data dependency chain:
  //       to get the node N+1 you need to retrieve the node N first.
  //       Think how you can execute multiple dependency chains in parallel.

  constexpr int num_partitions = 8;
// unsigned solution(List* l1, List* l2) {
//   unsigned retVal = 0;

//   List* head2 = l2;

//   List *fast = l2, *slow = l2;

//   while (fast != nullptr && fast->next != nullptr) {
//     slow = slow->next;
//     fast = fast->next->next;
//   }

//   List* mid = slow;

//   if (false) {
//     // O(N^2) algorithm:
//     while (l1) {
//       unsigned v = l1->value;
//       l2 = head2;
//       while (l2) {
//         if (l2->value == v) {
//           retVal += getSumOfDigits(v);
//           break;
//         }
//         l2 = l2->next;
//       }
//       l1 = l1->next;
//     }
//   } else {
//     while (l1) {
//       unsigned v = l1->value;
//       l2 = head2;

//       auto p1 = head2;
//       auto p2 = mid;

//       while (p1 != mid && p2 != nullptr) {
//         if (p1->value == v) {
//           retVal += getSumOfDigits(p1->value);
//         }
//         if (p2->value == v) {
//           retVal += getSumOfDigits(p2->value);
//         }

//         p2 = p2->next;
//         p1 = p1->next;
//       }

//       l1 = l1->next;
//     }
//   }
//   return retVal;
// }

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
#ifdef MULTI_OVERLAP
unsigned solution(List* l1, List* l2) {
  // printf("overlap solution\n");
  unsigned retVal = 0;

  List* head2 = l2;

  List* p[num_partitions];
  List* heads[num_partitions];
  int l2_len = 0;
  l2 = head2;
  while (l2) {
    l2_len++;
    l2 = l2->next;
  }

  int partition_cnt = l2_len / num_partitions;
  l2 = head2;
  for (int i = 0; i < l2_len; i++) {
    if (i % partition_cnt == 0) {
      heads[i / partition_cnt] = l2;
    }
    l2 = l2->next;
  }

  while (l1) {
    unsigned v = l1->value;
    l2 = head2;
    int processed = 0;

    for (int i = 0; i < num_partitions; i++) {
      p[i] = heads[i];
    }
    while (true) {
      
      int i;
      #pragma unroll 8
      for ( i = 0; i < num_partitions; i++) {
        if (p[i] != nullptr) {
          processed++;

          if (p[i]->value == v) {
            retVal += getSumOfDigits(v);
          }

          if (i + 1 < num_partitions && heads[i + 1] == p[i]->next) {
            p[i] = nullptr;
          } else {
            p[i] = p[i]->next;
          }
        }
      }

      // int i;
      // i=0;DO();
      // i=1;DO();
      // i=2;DO();
      // i=3;DO();
      // i=4;DO();
      // i=5;DO();
      // i=6;DO();
      // i=7;DO();

      if (processed == l2_len) {
        break;
      }
    }

    l1 = l1->next;
  }

  return retVal;
}


// unsigned solution(List* l1, List* l2) {
//   unsigned retVal = 0;

//   List* head2 = l2;

//   List* p[num_partitions];
//   List* heads[num_partitions];
//   int l2_len = 0;
//   l2 = head2;
//   while (l2) {
//     l2_len++;
//     l2 = l2->next;
//   }

//   int partition_cnt = l2_len / num_partitions;
//   l2 = head2;
//   for (int i = 0; i < l2_len; i++) {
//     if (i % partition_cnt == 0) {
//       heads[i / partition_cnt] = l2;
//     }
//     l2 = l2->next;
//   }

//   while (l1) {
//     unsigned v = l1->value;
//     l2 = head2;
//     auto exit = true;

//     for (int i = 0; i < num_partitions; i++) {
//       p[i] = heads[i];
//     }
//     do {
//       exit = true;
//       for (int i = 0; i < num_partitions; i++) {
//         if (p[i] != nullptr) {
//           exit = false;

//           if (p[i]->value == v) {
//             retVal += getSumOfDigits(v);
//           }

//           if (i + 1 < num_partitions && heads[i + 1] == p[i]->next) {
//             p[i] = nullptr;
//           } else {
//             p[i] = p[i]->next;
//           }
//         }
//       }
//     } while (!exit);

//     l1 = l1->next;
//   }

//   return retVal;
// }
#else
unsigned solution(List *l1, List *l2) {
  unsigned retVal = 0;

  List *head2 = l2;

  List *fast = l2, *slow = l2;

  while (fast != nullptr && fast->next != nullptr) {
    slow = slow->next;
    fast = fast->next->next;
  }

  List* mid = slow;

  // O(N^2) algorithm:
  // while (l1) {
  //   unsigned v = l1->value;
  //   l2 = head2;
  //   while (l2) {
  //     if (l2->value == v) {
  //       retVal += getSumOfDigits(v);
  //       break;
  //     }
  //     l2 = l2->next;
  //   }
  //   l1 = l1->next;
  // }

  while (l1) {
    unsigned v = l1->value;
    l2 = head2;

    auto p1 = head2;
    auto p2 = mid;

    while (p1 != mid && p2 != nullptr) {
      if (p1->value == v) {
        retVal += getSumOfDigits(p1->value);
      }
      if (p2->value == v) {
        retVal += getSumOfDigits(p2->value);
      }

      p2 = p2->next;
      p1 = p1->next;
    }

    l1 = l1->next;
  }
  return retVal;
}

#endif

#endif
