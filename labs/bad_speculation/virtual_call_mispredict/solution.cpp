#include "solution.h"

#include <random>

void generateObjects(InstanceArray& array) {
    std::default_random_engine generator(0);
    std::uniform_int_distribution<std::uint32_t> distribution(0, 2);

    InstanceArray tmpA, tmpB, tmpC;
    for (std::size_t i = 0; i < N; i++) {
        int value = distribution(generator);
        if (value == 0) {
            tmpA.push_back(std::make_unique<ClassA>());
        } else if (value == 1) {
            tmpB.push_back(std::make_unique<ClassB>());
        } else {
            tmpC.push_back(std::make_unique<ClassC>());
        }
    }

    array.reserve(tmpA.size() + tmpB.size() + tmpC.size());
    for (size_t i = 0; i < tmpA.size(); i++) {
        array.push_back(std::move(tmpA[i]));
    }
    for (size_t i = 0; i < tmpB.size(); i++) {
        array.push_back(std::move(tmpB[i]));
    }
    for (size_t i = 0; i < tmpC.size(); i++) {
        array.push_back(std::move(tmpC[i]));
    }
}

// Invoke the `handle` method on all instances in `output`
void invoke(InstanceArray& array, std::size_t& data) {
    for (const auto& item: array) {
        item->handle(data);
    }
}
