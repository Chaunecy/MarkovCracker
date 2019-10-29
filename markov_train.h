//
// Created by cw on 10/29/19.
//

#ifndef MARKOV_MARKOV_TRAIN_H
#define MARKOV_MARKOV_TRAIN_H

#include "include/cereal/types/unordered_map.hpp"
#include "include/cereal/types/memory.hpp"
#include "include/cereal/archives/binary.hpp"
#include "include/cereal/types/string.hpp"
#include <fstream>
#include <iostream>

struct MyRecord {
    uint8_t x, y;
    float z;

    template<class Archive>
    void serialize(Archive &ar) {
        ar(x, y, z);
    }
};

struct SomeData {
    int32_t id;
    std::shared_ptr<std::unordered_map<uint32_t, MyRecord>> data;

    template<class Archive>
    void save(Archive &ar) const {
        ar(data);
    }

    template<class Archive>
    void load(Archive &ar) {
        static int32_t idGen = 0;
        id = idGen++;
        ar(data);
    }
};

#endif //MARKOV_MARKOV_TRAIN_H
