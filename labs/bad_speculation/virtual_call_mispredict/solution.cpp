#include "solution.h"

#include <algorithm>
#include <random>

int getClassId(const BaseClass* obj) {
    if (dynamic_cast<ClassA const*>(obj) != nullptr) {
        return 0;
    }
    if (dynamic_cast<ClassB const*>(obj) != nullptr) {
        return 1;
    }
    if (dynamic_cast<ClassC const*>(obj) != nullptr) {
        return 2;
    }
    return 3;
}

void generateObjects(InstanceArray& array) {
    std::default_random_engine generator(0);
    std::uniform_int_distribution<std::uint32_t> distribution(0, 2);

    for (std::size_t i = 0; i < N; i++) {
        int value = distribution(generator);
        if (value == 0) {
            array.push_back(std::make_unique<ClassA>());
        } else if (value == 1) {
            array.push_back(std::make_unique<ClassB>());
        } else {
            array.push_back(std::make_unique<ClassC>());
        }
    }

    std::sort(array.begin(), array.end(), [](const std::unique_ptr<BaseClass>& left, const std::unique_ptr<BaseClass>& right) {
        return getClassId(left.get()) < getClassId(right.get());
    });
}

// Invoke the `handle` method on all instances in `output`
void invoke(InstanceArray& array, std::size_t& data) {
    for (const auto& item: array) {
        item->handle(data);
    }
}
