#include "solution.h"

#include <random>

void generateObjects(InstanceArray& array) {
    std::default_random_engine generator(0);
    std::uniform_int_distribution<std::uint32_t> distribution(0, 2);

    InstanceArray array1, array2, array3;
    for (std::size_t i = 0; i < N; i++) {
        int value = distribution(generator);
        if (value == 0) {
            array1.push_back(std::make_unique<ClassA>());
        } else if (value == 1) {
            array2.push_back(std::make_unique<ClassB>());
        } else {
            array3.push_back(std::make_unique<ClassC>());
        }
    }
    array.insert(array.end(), std::make_move_iterator(array1.begin()), std::make_move_iterator(array1.end()));
    array.insert(array.end(), std::make_move_iterator(array2.begin()), std::make_move_iterator(array2.end()));
    array.insert(array.end(), std::make_move_iterator(array3.begin()), std::make_move_iterator(array3.end()));
}

// Invoke the `handle` method on all instances in `output`
void invoke(InstanceArray& array, std::size_t& data) {
    for (const auto& item: array) {
        item->handle(data);
    }
}
