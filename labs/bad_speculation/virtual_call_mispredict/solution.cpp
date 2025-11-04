#include "solution.h"

#include <random>

void generateObjects(InstanceArray& array) {
    std::default_random_engine generator(0);
    std::uniform_int_distribution<std::uint32_t> distribution(0, 2);

    for (std::size_t i = 0; i < N; i++) {
        int value = distribution(generator);
        
        BaseClass *obj = nullptr;

        if (value == 0) {
            obj = new ClassA();
        } else if (value == 1) {
            obj = new ClassB();
        } else {
            obj = new ClassC();
        }

        array.push_back(std::unique_ptr<BaseClass>(obj));
    
    }
}

// Invoke the `handle` method on all instances in `output`
void invoke(InstanceArray& array, std::size_t& data) {
    for (const auto& item: array) {
        item->handle(data);
    }
}
