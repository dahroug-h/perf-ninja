#include "solution.h"

#include <random>

void generateObjects(InstanceArray& array) {
    std::default_random_engine generator(0);
    std::uniform_int_distribution<std::uint32_t> distribution(0, 2);

    for (std::size_t i = 0; i < N; i++) {
        int value = distribution(generator);

        switch (value) {
            case 0:
                array.push_back(std::make_unique<ClassA>());
                break;
            case 1:
                array.push_back(std::make_unique<ClassB>());
                break;
            default:
                array.push_back(std::make_unique<ClassC>());
                break;
        }
    }
}

// Invoke the `handle` method on all instances in `output`
void invoke(InstanceArray& array, std::size_t& data) {
    for (const auto& item: array) {
        item->handle(data);
    }
}
