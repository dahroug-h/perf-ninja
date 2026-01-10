#include "solution.h"
#define SOLUTION
#include <random>
void generateObjects(InstanceArray& array) {
  std::default_random_engine generator(0);
  std::uniform_int_distribution<std::uint32_t> distribution(0, 2);
#ifdef SOLUTION
  std::vector<std::unique_ptr<BaseClass>> v1, v2, v3;
#endif
  for (std::size_t i = 0; i < N; i++) {
    int value = distribution(generator);
#ifdef SOLUTION

    if (value == 0) {
      v1.push_back(std::make_unique<ClassA>());
    } else if (value == 1) {
      v2.push_back(std::make_unique<ClassB>());
    } else {
      v3.push_back(std::make_unique<ClassC>());
    }
#else
    if (value == 0) {
      array.push_back(std::make_unique<ClassA>());
    } else if (value == 1) {
      array.push_back(std::make_unique<ClassB>());
    } else {
      array.push_back(std::make_unique<ClassC>());
    }
#endif
  }
#ifdef SOLUTION
  for (int i = 0; i < v1.size(); i++) {
    array.push_back(std::move(v1[i]));
  }

  for (int i = 0; i < v2.size(); i++) {
    array.push_back(std::move(v2[i]));
  }

  for (int i = 0; i < v3.size(); i++) {
    array.push_back(std::move(v3[i]));
  }
#endif
}

// Invoke the `handle` method on all instances in `output`
void invoke(InstanceArray& array, std::size_t& data) {
  for (const auto& item : array) {
    item->handle(data);
  }
}
